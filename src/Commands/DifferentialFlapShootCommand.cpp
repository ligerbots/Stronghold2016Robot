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
	m_intakeFinished = false;
	m_ticksSinceFire = 0;
	m_die = false;
}

void DifferentialFlapShootCommand::Initialize() {
	printf("DifferentialFlapShootCommand: init\n");
	m_state = GET_FRAME;
	m_die = false;
	visionSubsystem->requestVisionFrame();
	m_intakeFinished = false;
	m_ticksSinceFire = 0;
	mp_rollerCommand->Initialize();
}

void DifferentialFlapShootCommand::Execute() {
	// run intake in parallel. INTAKE_FLAPS will wait for this to complete before going into SHOOTING
	if(!m_intakeFinished){
		mp_rollerCommand->Execute();
		if(mp_rollerCommand->IsFinished()){
			m_intakeFinished = true;
			mp_rollerCommand->End();
			intakeSubsystem->setIntakeArmUp();
		}
	}

	bool success;

	switch(m_state) {
	case GET_FRAME:
		if(!visionSubsystem->isVisionBusy()){
			if(fabs(visionSubsystem->TargetFineAngle()) < FieldInfo::flapDataMaxAngle){
				m_state = INTAKE_FLAPS;
				flapSubsystem->setFlapsDifferential(
						visionSubsystem->getDistanceToTarget(), visionSubsystem->TargetFineAngle());
			} else {
				m_state = CENTERING;
				mp_centeringCommand->SetUseFineAngle(fabs(visionSubsystem->TargetAngle()) < 7);
				mp_centeringCommand->Initialize();
			}
		}
		break;
	case CENTERING:
		mp_centeringCommand->Execute();
		if(mp_centeringCommand->IsFinished()){
			mp_centeringCommand->End();
			m_state = GET_FRAME;
			visionSubsystem->requestVisionFrame();
		}
		break;
	case INTAKE_FLAPS:
		visionSubsystem->requestVisionFrame();
		success = flapSubsystem->setFlapsDifferential(
				visionSubsystem->getDistanceToTarget(), visionSubsystem->TargetFineAngle());
		if(!success) {
			m_die = true;
		} else if(m_intakeFinished && intakeSubsystem->isBallInShooterPosition()){
			m_state = SHOOTING;
			m_ticksSinceFire = 0;
			shooterSubsystem->firePiston();
		}
		// if we got hit and the ball fell out, don't shoot
		if(m_intakeFinished && !intakeSubsystem->isBallInShooterPosition()){
			m_die = true;
		}
		break;
	case SHOOTING:
		m_ticksSinceFire++;
		break;
	}
}

bool DifferentialFlapShootCommand::IsFinished() {
	if(m_die){
		printf("DifferentialFlapShootCommand: exiting because robot was pushed\n");
		return true;
	}
	if (Robot::instance->mp_operatorInterface->getSecondControllerRawButton(28)) {
		printf("DifferentialFlapShootCommand: canceling\n");
		return true;
	}
	return m_state == SHOOTING && m_ticksSinceFire > 25;
}

void DifferentialFlapShootCommand::End() {
	printf("DifferentialFlapShootCommand: end\n");
	shooterSubsystem->retractPiston();
	flapCommand->Start();
	driveJoystickCommand->Start();
	intakeRollerCommand->Start();
}

void DifferentialFlapShootCommand::Interrupted() {
	printf("DifferentialFlapShootCommand: interrupted\n");
}
