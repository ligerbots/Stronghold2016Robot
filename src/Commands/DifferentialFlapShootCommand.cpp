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
	m_RTCWhenFlapSet = -1;
	m_die = false;
	m_yawAtCenteringEnd = 0;
	m_haveFlapsBeenSet = false;
	m_RTCWhenIntakeFinished = 0;
}

void DifferentialFlapShootCommand::Initialize() {
	printf("DifferentialFlapShootCommand: init\n");
	m_state = GET_FRAME;
	printf("DifferentialFlapShootCommand: GET_FRAME\n");
	m_die = false;
	visionSubsystem->requestVisionFrame();
	m_intakeFinished = false;
	m_ticksSinceFire = 0;
	m_RTCWhenFlapSet = -1;
	m_yawAtCenteringEnd = 0;
	m_haveFlapsBeenSet = false;
	m_RTCWhenIntakeFinished = 0;
	mp_rollerCommand->Initialize();
}

void DifferentialFlapShootCommand::Execute() {
	// run intake in parallel. INTAKE_FLAPS will wait for this to complete before going into SHOOTING
	if(!m_intakeFinished){
		mp_rollerCommand->Execute();
		if(mp_rollerCommand->IsFinished()){
			m_intakeFinished = true;
			m_RTCWhenIntakeFinished = Robot::GetRTC();
			mp_rollerCommand->End();
			intakeSubsystem->setIntakeArmUp();
			printf("DifferentialFlapShootCommand: intake rolling ended; intake arm up\n");
		}
	}

	bool success;
	double yawDiff;
	double rtcNow;
	bool ballInShooter;

	switch(m_state) {
	case GET_FRAME:
		if(!visionSubsystem->isVisionBusy()){
			if(fabs(visionSubsystem->TargetFineAngle()) < 2){
				m_state = INTAKE_FLAPS;
				m_RTCWhenFlapSet = -1;
				m_yawAtCenteringEnd = navXSubsystem->GetYaw();
				printf("DifferentialFlapShootCommand: INTAKE_FLAPS\n");
			} else {
				m_state = CENTERING;
				printf("DifferentialFlapShootCommand: CENTERING\n");
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
			printf("DifferentialFlapShootCommand: GET_FRAME\n");
			visionSubsystem->requestVisionFrame();
		}
		break;
	case INTAKE_FLAPS:
		visionSubsystem->requestVisionFrame();
		success = true;
		if(m_intakeFinished){
			success = flapSubsystem->setFlapsDifferential(
					visionSubsystem->getDistanceToTarget(), visionSubsystem->TargetFineAngle());
			if(m_RTCWhenFlapSet < 0){
				m_RTCWhenFlapSet = Robot::GetRTC();
			}
			if(success){
				m_haveFlapsBeenSet = true;
			}
		}
		// double check using navx in case vision doesn't get a frame in time
		// navx can go into a sudden .1deg/sec drift so the margin for the check is high
		yawDiff = fabs(m_yawAtCenteringEnd - navXSubsystem->GetYaw());
		if(yawDiff > 180){
			yawDiff -= 180;
		}
		// take the shot anyway in auto
		// added after Hartford Q3
		rtcNow = Robot::GetRTC();
		ballInShooter = intakeSubsystem->isBallInShooterPosition();

		if(!(DriverStation::GetInstance().IsAutonomous() && m_haveFlapsBeenSet)
				&& (!success || yawDiff > 5.0)) {
			m_die = true;
			printf("DifferentialFlapShootCommand: die because flap interpolation failed\n");
		} else if(m_intakeFinished && intakeSubsystem->isIntakeArmUp() && ballInShooter && rtcNow - m_RTCWhenFlapSet > .2){
			if(intakeSubsystem->isBallInDefensesCrossingPosition()){ // BAD, regardless of the cable coming loose
				m_die = true;
				printf("DifferentialFlapShootCommand: die because ball hit defense crossing switch\n");
			} else {
				m_state = SHOOTING;
				printf("DifferentialFlapShootCommand: SHOOTING\n");
				m_ticksSinceFire = 0;
				shooterSubsystem->firePiston();
				Robot::instance->SetLeds(Robot::SHOOT);
			}
		}
		// if we got hit and the ball fell out, don't shoot
		// wait for bouncing for for .5 secs
		if(m_intakeFinished && rtcNow - m_RTCWhenIntakeFinished > 0.5 && !ballInShooter){
			m_die = true;
			printf("DifferentialFlapShootCommand: die because ball is not in shooter\n");
		}
		break;
	case SHOOTING:
		m_ticksSinceFire++;
		break;
	}
}

bool DifferentialFlapShootCommand::IsFinished() {
	if(m_die){
		printf("DifferentialFlapShootCommand: exiting because m_die == true\n");
		return true;
	}
	if (Robot::instance->mp_operatorInterface->getSecondControllerRawButton(28)) {
		printf("DifferentialFlapShootCommand: canceling because kill button pressed\n");
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
