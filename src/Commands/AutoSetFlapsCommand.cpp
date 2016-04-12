#include <Stronghold2016Robot.h>

AutoSetFlapsCommand::AutoSetFlapsCommand() {
	Requires(flapSubsystem.get());
	m_flapsFractionToSet = 0;
	m_isDifferential = false;
}

AutoSetFlapsCommand::AutoSetFlapsCommand(bool isDifferential) {
	Requires(flapSubsystem.get());
	m_flapsFractionToSet = 0;
	m_isDifferential = isDifferential;
}

void AutoSetFlapsCommand::Initialize() {
	SetTimeout(0.25); // enough time for servos to get into position
	if(!m_isDifferential) {
		m_flapsFractionToSet = visionSubsystem->getFlapsFractionForDistance(visionSubsystem->getDistanceToTarget());
	}
}

void AutoSetFlapsCommand::Execute() {
	if(m_isDifferential) {
		flapSubsystem->setFlapsDifferential(
				visionSubsystem->getDistanceToTarget(), visionSubsystem->TargetFineAngle());
	} else {
		flapSubsystem->setFlapsFraction(m_flapsFractionToSet);
	}
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
