#include <Stronghold2016Robot.h>

RollBallToShooterCommand::RollBallToShooterCommand() :
		CommandBase("RollBallToShooter") {
	Requires(intakeSubsystem.get());
}

void RollBallToShooterCommand::Initialize() {
	SetTimeout(2);
}

void RollBallToShooterCommand::Execute() {
	// make sure this doesn't move the rollers on the first execute if the ball is already there
	if(!intakeSubsystem->isBallInShooterPosition())
		intakeSubsystem->setRollSpeed(0.5); // roll slowly (TODO: test and adjust this value)
}

bool RollBallToShooterCommand::IsFinished() {
	return IsTimedOut() || intakeSubsystem->isBallInShooterPosition();
}

void RollBallToShooterCommand::End() {
	intakeSubsystem->rollStop();
}

void RollBallToShooterCommand::Interrupted() {
	intakeSubsystem->rollStop();
}
