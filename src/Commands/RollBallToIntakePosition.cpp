#include <Stronghold2016Robot.h>

RollBallToIntakePositionCommand::RollBallToIntakePositionCommand(IntakePosition where) :
		CommandBase("RollBallToShooter"), where(where), sensorFlag(false), moveUp(false), ticks(0) {
	Requires(intakeSubsystem.get());
	Requires(flapSubsystem.get());
	ticks_since_shooter_switch = 0;
}

void RollBallToIntakePositionCommand::Initialize() {
	printf("RollBallToIntakePositionCommand: init\n");
	if(where != PICKUP){
		SetTimeout(2);
	}

	if(where == LOW_GOAL
			|| where == CROSSING_POSITION
			|| where == BACK_TO_SHOOTING_POSITION){
		moveUp = false;
	} else {
		moveUp = true;
	}
	flapSubsystem->setFlapsFraction(1); // all the way down
	intakeSubsystem->setIntakeArmDown();
}

void RollBallToIntakePositionCommand::Execute() {
	sensorFlag = false;

	if(where == CROSSING_POSITION || where == PICKUP){
		sensorFlag = intakeSubsystem->isBallInDefensesCrossingPosition();
	} else if(where == SHOOTING_POSITION || where == BACK_TO_SHOOTING_POSITION){
		sensorFlag = intakeSubsystem->isBallInShooterPosition();
	} // else, always false for low goal shot

	if(where == SHOOTING_POSITION && sensorFlag){
		ticks_since_shooter_switch++;
	} else {
		ticks_since_shooter_switch = 0;
	}

	ticks++;
	if(ticks < 20)
		return; // wait for flaps

	double rollSpeed = 0.3;
	if(where == LOW_GOAL || where == PICKUP)
		rollSpeed = 1; // max speed

	// make sure this doesn't move the rollers on the first execute if the ball is already there
	if(!sensorFlag)
		intakeSubsystem->setRollSpeed(moveUp ? rollSpeed : -rollSpeed); // roll slowly (TODO: test and adjust this value)
}

bool RollBallToIntakePositionCommand::IsFinished() {
	if(where == SHOOTING_POSITION){
		return IsTimedOut() || (sensorFlag && ticks_since_shooter_switch > 10);
	} else
		return IsTimedOut() || sensorFlag;
}

void RollBallToIntakePositionCommand::End() {
	intakeSubsystem->rollStop();
	if(DriverStation::GetInstance().IsOperatorControl() && this->GetGroup() == NULL){
		CommandBase::intakeRollerCommand->Start();
		CommandBase::flapCommand->Start();
	}
}

void RollBallToIntakePositionCommand::Interrupted() {
	printf("RollBallToIntakePositionCommand: interrupted\n");
	intakeSubsystem->rollStop();
	if(DriverStation::GetInstance().IsOperatorControl() && this->GetGroup() == NULL){
		CommandBase::intakeRollerCommand->Start();
		CommandBase::flapCommand->Start();
	}
}
