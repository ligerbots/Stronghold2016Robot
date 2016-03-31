#include <Stronghold2016Robot.h>

AutoSetFlapsCommand::AutoSetFlapsCommand() {
	Requires(flapSubsystem.get());
	m_flapsFractionToSet = 0;
}

void AutoSetFlapsCommand::Initialize() {
	SetTimeout(0.25); // enough time for servos to get into position
	m_flapsFractionToSet = visionSubsystem->getFlapsFractionForDistance(visionSubsystem->getDistanceToTarget());
}

void AutoSetFlapsCommand::Execute() {
	flapSubsystem->setFlapsFraction(m_flapsFractionToSet);
}

bool AutoSetFlapsCommand::IsFinished() {
	return IsTimedOut();
}

void AutoSetFlapsCommand::End() {
	if(DriverStation::GetInstance().IsOperatorControl() && this->GetGroup() == NULL){
		CommandBase::flapCommand->Start();
	}
}

void AutoSetFlapsCommand::Interrupted() {
}
