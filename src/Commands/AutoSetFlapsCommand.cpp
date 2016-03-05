#include <Stronghold2016Robot.h>

AutoSetFlapsCommand::AutoSetFlapsCommand() {
	Requires(flapSubsystem.get());
	flapsFractionToSet = 0;
}

void AutoSetFlapsCommand::Initialize() {
	SetTimeout(0.5); // enough time for servos to get into position
	flapsFractionToSet = visionSubsystem->getFlapsFractionForDistance(visionSubsystem->getDistanceToTarget());
}

void AutoSetFlapsCommand::Execute() {
	flapSubsystem->setFlapsFraction(flapsFractionToSet);
}

bool AutoSetFlapsCommand::IsFinished() {
	return IsTimedOut();
}

void AutoSetFlapsCommand::End() {

}

void AutoSetFlapsCommand::Interrupted() {
}
