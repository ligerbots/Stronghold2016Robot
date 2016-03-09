#include <Stronghold2016Robot.h>

AutoSetFlapsCommand::AutoSetFlapsCommand() {
	Requires(flapSubsystem.get());
	flapsFractionToSet = 0;
}

void AutoSetFlapsCommand::Initialize() {
	SetTimeout(0.25); // enough time for servos to get into position
	flapsFractionToSet = visionSubsystem->getFlapsFractionForDistance(visionSubsystem->getDistanceToTarget());
}

void AutoSetFlapsCommand::Execute() {
	flapSubsystem->setFlapsFraction(flapsFractionToSet);
}

bool AutoSetFlapsCommand::IsFinished() {
	return IsTimedOut();
}

void AutoSetFlapsCommand::End() {
	if(DriverStation::GetInstance().IsOperatorControl()){
		CommandBase::flapCommand->Start();
	}
}

void AutoSetFlapsCommand::Interrupted() {
	if(DriverStation::GetInstance().IsOperatorControl()){
		CommandBase::flapCommand->Start();
	}
}
