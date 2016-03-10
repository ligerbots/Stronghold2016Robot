#include <Stronghold2016Robot.h>

RollBallToIntakePositionCommand::RollBallToIntakePositionCommand(IntakePosition where) :
		CommandBase("RollBallToShooter"), where(where), sensorFlag(false), moveUp(false), ticks(0) {
	Requires(intakeSubsystem.get());
	Requires(flapSubsystem.get());
}

void RollBallToIntakePositionCommand::Initialize() {
	printf("RollBallToIntakePositionCommand: init\n");
	SetTimeout(3);

	if(where == LOW_GOAL
			|| (intakeSubsystem->isBallInShooterPosition() && where == CROSSING_POSITION)
			|| where == BACK_TO_SHOOTING_POSITION){
		moveUp = false;
	} else {
		moveUp = true;
	}
	flapSubsystem->setFlapsFraction(1); // all the way down
}

void RollBallToIntakePositionCommand::Execute() {
	sensorFlag = false;

	if(where == CROSSING_POSITION){
		sensorFlag = intakeSubsystem->isBallInDefensesCrossingPosition();
	} else if(where == SHOOTING_POSITION || where == BACK_TO_SHOOTING_POSITION){
		sensorFlag = intakeSubsystem->isBallInShooterPosition();
	} // else, always false for low goal shot

	ticks++;
	if(ticks < 10)
		return; // wait for flaps

	double rollSpeed = moveUp ? -0.5: 0.5;
	if(where == LOW_GOAL)
		rollSpeed = 1; // max speed

	// make sure this doesn't move the rollers on the first execute if the ball is already there
	if(!sensorFlag)
		intakeSubsystem->setRollSpeed(rollSpeed); // roll slowly (TODO: test and adjust this value)
}

bool RollBallToIntakePositionCommand::IsFinished() {
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
