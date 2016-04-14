#include <Stronghold2016Robot.h>

PrepareForAutoCommand::PrepareForAutoCommand(bool wedgesUp, bool intakeUp) :
		CommandBase("PrepareForAutoCommand"), m_setWedgesUp(wedgesUp), m_setIntakeUp(
				intakeUp) {
	Requires(wedgeSubsystem.get());
	Requires(intakeSubsystem.get());
}

void PrepareForAutoCommand::Initialize() {
	printf("PrepareForAuto: setting wedges %s | intake %s\n",
			m_setWedgesUp ? "up" : "down", m_setIntakeUp ? "up" : "down");
	visionSubsystem->setVisionEnabled(true);
	SetTimeout(0.1); // give enough time for things to go into position
}

void PrepareForAutoCommand::Execute() {
	if (m_setWedgesUp)
		wedgeSubsystem->liftWedge();
	else
		wedgeSubsystem->lowerWedge();

	if (m_setIntakeUp)
		intakeSubsystem->setIntakeArmUp();
	else
		intakeSubsystem->setIntakeArmDown();
}

bool PrepareForAutoCommand::IsFinished() {
	return IsTimedOut();
}

void PrepareForAutoCommand::End() {
	printf("PrepareForAuto: end\n");
}

void PrepareForAutoCommand::Interrupted() {
	printf("PrepareForAuto: interrupted\n");
}
