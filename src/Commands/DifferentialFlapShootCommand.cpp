#include "DifferentialFlapShootCommand.h"

DifferentialFlapShootCommand::DifferentialFlapShootCommand() {
	Requires(shooterSubsystem.get());
	Requires(flapSubsystem.get());
	Requires(driveSubsystem.get());
	mp_rollerCommand = new RollBallToIntakePositionCommand(
			RollBallToIntakePositionCommand::SHOOTING_POSITION);
	mp_rollerCommand->m_restartRollerCommand = false; // a bad way to do this in terms of design
	mp_centeringCommand = new RotateToTarget();
	mp_centeringCommand->m_restartDriveCommand = false;
	m_state = GET_FRAME;
	m_rollFinished = false;
	m_ticksSinceFire = 0;
}

void DifferentialFlapShootCommand::Initialize() {
	printf("DifferentialFlapShootCommand: init\n");
	m_state = GET_FRAME;
	visionSubsystem->requestVisionFrame();
	m_rollFinished = false;
	m_ticksSinceFire = 0;
	mp_rollerCommand->Initialize();
}

void DifferentialFlapShootCommand::Execute() {
	if(!m_rollFinished){
		mp_rollerCommand->Execute();
		if(mp_rollerCommand->IsFinished()){
			m_rollFinished = true;
			mp_rollerCommand->End();
		}
	}
	switch(m_state) { // TODO: finish this
	case GET_FRAME:
		if(!visionSubsystem->isVisionBusy()){
			if(fabs(visionSubsystem->TargetAngle()) < 5){
				m_state = INTAKE_UP;
				intakeSubsystem->setIntakeArmUp();
				// TODO: add the differential flaps stuff here
				// flapSubsystem->setFlapsFractionDifferential(0, 0);
			} else {
				m_state = CENTERING;
				mp_centeringCommand->Initialize();
			}
		}
		break;
	case CENTERING:
		mp_centeringCommand->Execute();
		if(mp_centeringCommand->IsFinished()){
			mp_centeringCommand->End();
			m_state = GET_FRAME;
		}
		break;
	case INTAKE_UP:
		if(intakeSubsystem->isIntakeArmUp() && m_rollFinished){
			m_state = SHOOTING;
			m_ticksSinceFire = 0;
			shooterSubsystem->firePistons();
		}
		break;
	case SHOOTING:
		m_ticksSinceFire++;
		break;
	}
}

bool DifferentialFlapShootCommand::IsFinished() {
	if (Robot::instance->mp_operatorInterface->getSecondControllerRawButton(28)) {
		printf("DifferentialFlapShootCommand: canceling\n");
		return true;
	}
	return m_state == SHOOTING && m_ticksSinceFire > 25;
}

void DifferentialFlapShootCommand::End() {
	printf("DifferentialFlapShootCommand: end\n");
	shooterSubsystem->retractPistons();
	flapCommand->Start();
	driveJoystickCommand->Start();
	intakeRollerCommand->Start();
}

void DifferentialFlapShootCommand::Interrupted() {
	printf("DifferentialFlapShootCommand: interrupted\n");
}
