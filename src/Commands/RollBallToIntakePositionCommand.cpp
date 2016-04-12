#include <Stronghold2016Robot.h>

RollBallToIntakePositionCommand::RollBallToIntakePositionCommand(IntakePosition targetIntakePositon) :
		CommandBase("RollBallToShooter"), m_targetIntakePosition(targetIntakePositon), m_sensorFlag(false), m_moveUp(false), m_ticks(0) {
	Requires(intakeSubsystem.get());
	Requires(flapSubsystem.get());
	m_shooterSwitchState = STATE_UP;
	m_waitingTicks = 0;
	m_ticksSinceCrossingPosition = 0;
	m_needsToWaitForFlaps = true;
	m_switchAlreadyPressed = false;
	m_restartRollerCommand = true;
//	SetInterruptible(false);
}

void RollBallToIntakePositionCommand::Initialize() {
	printf("RollBallToIntakePositionCommand: init\n");
//	SetInterruptible(false);
	if(m_targetIntakePosition != PICKUP){
		SetTimeout(6);
	}

	if(m_targetIntakePosition == LOW_GOAL){
		SetTimeout(3);
	}

	intakeSubsystem->rollStop();

	if(m_targetIntakePosition == SHOOTING_POSITION){
		m_shooterSwitchState = STATE_UP;
	}

	if(m_targetIntakePosition == LOW_GOAL
			|| m_targetIntakePosition == CROSSING_POSITION
			|| m_targetIntakePosition == BACK_TO_SHOOTING_POSITION){
		m_moveUp = false;
	} else {
		m_moveUp = true;
	}

	if(flapSubsystem->isSafeToIntake()){
		m_needsToWaitForFlaps = false;
		printf("Skipping flaps wait\n");
	} else {
		flapSubsystem->setFlapsFraction(1); // all the way down
		m_needsToWaitForFlaps = true;
		printf("Waiting for flaps\n");
	}
	intakeSubsystem->setIntakeArmDown();

	if(m_targetIntakePosition == SHOOTING_POSITION){
		m_switchAlreadyPressed = intakeSubsystem->isBallInShooterPosition();
	}

	m_ticks = 0;
	m_ticksSinceCrossingPosition = 0;
}

void RollBallToIntakePositionCommand::Execute() {
	m_sensorFlag = false;

	if(m_targetIntakePosition == CROSSING_POSITION || m_targetIntakePosition == PICKUP){
		m_sensorFlag = intakeSubsystem->isBallInDefensesCrossingPosition();
	} else if(m_targetIntakePosition == SHOOTING_POSITION || m_targetIntakePosition == BACK_TO_SHOOTING_POSITION){
		m_sensorFlag = intakeSubsystem->isBallInShooterPosition();
	} // else, always false for low goal shot

	if(m_targetIntakePosition == CROSSING_POSITION && m_sensorFlag){
		m_ticksSinceCrossingPosition++;
	} else {
		m_ticksSinceCrossingPosition = 0;
	}

	m_ticks++;
	if(m_needsToWaitForFlaps && !flapSubsystem->isSafeToIntake()){
		return; // wait for flaps to go down
	}

	double rollSpeed = .5;
	if(m_targetIntakePosition == LOW_GOAL || m_targetIntakePosition == PICKUP)
		rollSpeed = 1; // max speed

	if(m_targetIntakePosition == SHOOTING_POSITION){
		// implement a small state machine for rolling the ball to the shooter
		// 1a. Roll ball up until shooter switch pressed, then go to 2 (STATE_UP)
		// 1b. If it takes too long, go directly to 3
		// 2. Keep rolling until released, then go to 3 (STATE_WAIT_FOR_RELEASE)
		// 3. Roll back until pressed again (STATE_BACK)
		if(m_shooterSwitchState == STATE_BACK){
			m_moveUp = false;
		} else {
			m_moveUp = true;
		}
		int ticksTimeout = 45;
		if(m_needsToWaitForFlaps) {
			ticksTimeout += 20; // add additional wait time for flaps to get into position
		}
		if(m_ticks > ticksTimeout && m_shooterSwitchState == STATE_UP){
			m_shooterSwitchState = STATE_BACK;
			printf("RollBall: State = STATE_BACK\n");
		}
		if(m_shooterSwitchState == STATE_UP && m_sensorFlag){
			m_shooterSwitchState = STATE_WAIT_FOR_RELEASE;
			printf("RollBall: State = STATE_WAIT_FOR_RELEASE\n");
		} else if(m_shooterSwitchState == STATE_WAIT_FOR_RELEASE && !m_sensorFlag){
			m_shooterSwitchState = STATE_WAIT;
			printf("RollBall: State = STATE_WAIT\n");
			m_waitingTicks = 0;
		} else if(m_shooterSwitchState == STATE_WAIT){
			m_waitingTicks++;
			if(m_waitingTicks > 5){
				m_shooterSwitchState = STATE_BACK;
				printf("RollBall: State = STATE_BACK\n");
			}
		}
	}

	// make sure this doesn't move the rollers on the first execute if the ball is already there
	if(!m_sensorFlag)
		intakeSubsystem->setRollSpeed(m_moveUp ? rollSpeed : -rollSpeed); // roll slowly (TODO: test and adjust this value)
}

bool RollBallToIntakePositionCommand::IsFinished() {
	if(m_targetIntakePosition == SHOOTING_POSITION){
		return IsTimedOut() ||
				m_switchAlreadyPressed ||
				(m_shooterSwitchState == STATE_BACK && m_sensorFlag);
	} else if(m_targetIntakePosition == CROSSING_POSITION){
		return IsTimedOut() || (m_sensorFlag && m_ticksSinceCrossingPosition > 5);
	} else
		return IsTimedOut() || m_sensorFlag;
}

void RollBallToIntakePositionCommand::End() {
	printf("RollBallToIntakePositionCommand: End\n");
	intakeSubsystem->rollStop();
	if(m_restartRollerCommand && DriverStation::GetInstance().IsOperatorControl()
			&& this->GetGroup() == NULL){
		CommandBase::intakeRollerCommand->Start();
		CommandBase::flapCommand->Start();
	}
}

void RollBallToIntakePositionCommand::Interrupted() {
	printf("RollBallToIntakePositionCommand: interrupted\n");
	intakeSubsystem->rollStop();
}
