#include <Stronghold2016Robot.h>

RollBallToIntakePositionCommand::RollBallToIntakePositionCommand(IntakePosition where) :
		CommandBase("RollBallToShooter"), where(where), sensorFlag(false), moveUp(false), ticks(0) {
	Requires(intakeSubsystem.get());
	Requires(flapSubsystem.get());
	shooter_switch_state = STATE_UP;
	waiting_ticks = 0;
	ticks_since_crossing_position = 0;
	needsToWaitForFlaps = true;
	switchAlreadyPressed = false;
//	SetInterruptible(false);
}

void RollBallToIntakePositionCommand::Initialize() {
	printf("RollBallToIntakePositionCommand: init\n");
//	SetInterruptible(false);
	if(where != PICKUP){
		SetTimeout(6);
	}

	if(where == LOW_GOAL){
		SetTimeout(3);
	}

	intakeSubsystem->rollStop();

	if(where == SHOOTING_POSITION){
		shooter_switch_state = STATE_UP;
	}

	if(where == LOW_GOAL
			|| where == CROSSING_POSITION
			|| where == BACK_TO_SHOOTING_POSITION){
		moveUp = false;
	} else {
		moveUp = true;
	}

	if(flapSubsystem->isSafeToIntake()){
		needsToWaitForFlaps = false;
		printf("Skipping flaps wait\n");
	} else {
		flapSubsystem->setFlapsFraction(1); // all the way down
		needsToWaitForFlaps = true;
		printf("Waiting for flaps\n");
	}
	intakeSubsystem->setIntakeArmDown();

	if(where == SHOOTING_POSITION){
		switchAlreadyPressed = intakeSubsystem->isBallInShooterPosition();
	}

	ticks = 0;
	ticks_since_crossing_position = 0;
}

void RollBallToIntakePositionCommand::Execute() {
	sensorFlag = false;

	if(where == CROSSING_POSITION || where == PICKUP){
		sensorFlag = intakeSubsystem->isBallInDefensesCrossingPosition();
	} else if(where == SHOOTING_POSITION || where == BACK_TO_SHOOTING_POSITION){
		sensorFlag = intakeSubsystem->isBallInShooterPosition();
	} // else, always false for low goal shot

	if(where == CROSSING_POSITION && sensorFlag){
		ticks_since_crossing_position++;
	} else {
		ticks_since_crossing_position = 0;
	}

	ticks++;
	if(needsToWaitForFlaps && !flapSubsystem->isSafeToIntake())
		return; // wait for flaps

	double rollSpeed = .5;
	if(where == LOW_GOAL || where == PICKUP)
		rollSpeed = 1; // max speed

	if(where == SHOOTING_POSITION){
		if(shooter_switch_state == STATE_BACK){
			moveUp = false;
		} else {
			moveUp = true;
		}
		int ticksTimeout = 45;
		if(needsToWaitForFlaps)
			ticksTimeout += 20;
		if(ticks > ticksTimeout && shooter_switch_state == STATE_UP){
			shooter_switch_state = STATE_BACK;
			printf("RollBall: State = STATE_BACK\n");
		}
		if(shooter_switch_state == STATE_UP && sensorFlag){
			shooter_switch_state = STATE_WAIT_FOR_RELEASE;
			printf("RollBall: State = STATE_WAIT_FOR_RELEASE\n");
		} else if(shooter_switch_state == STATE_WAIT_FOR_RELEASE && !sensorFlag){
			shooter_switch_state = STATE_WAIT;
			printf("RollBall: State = STATE_WAIT\n");
			waiting_ticks = 0;
		} else if(shooter_switch_state == STATE_WAIT){
			waiting_ticks++;
			if(waiting_ticks > 5){
				shooter_switch_state = STATE_BACK;
				printf("RollBall: State = STATE_BACK\n");
			}
		}
	}

	// make sure this doesn't move the rollers on the first execute if the ball is already there
	if(!sensorFlag)
		intakeSubsystem->setRollSpeed(moveUp ? rollSpeed : -rollSpeed); // roll slowly (TODO: test and adjust this value)
}

bool RollBallToIntakePositionCommand::IsFinished() {
	if(where == SHOOTING_POSITION){
		return IsTimedOut() ||
				switchAlreadyPressed ||
				(shooter_switch_state == STATE_BACK && sensorFlag);
	} else if(where == CROSSING_POSITION){
		return IsTimedOut() || (sensorFlag && ticks_since_crossing_position > 5);
	} else
		return IsTimedOut() || sensorFlag;
}

void RollBallToIntakePositionCommand::End() {
	printf("RollBallToIntakePositionCommand: End\n");
	intakeSubsystem->rollStop();
	if(DriverStation::GetInstance().IsOperatorControl() && this->GetGroup() == NULL){
		CommandBase::intakeRollerCommand->Start();
		CommandBase::flapCommand->Start();
	}
}

void RollBallToIntakePositionCommand::Interrupted() {
	printf("RollBallToIntakePositionCommand: interrupted\n");
	intakeSubsystem->rollStop();
}
