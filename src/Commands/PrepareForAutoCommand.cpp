#include <Stronghold2016Robot.h>

PrepareForAutoCommand::PrepareForAutoCommand(bool wedgesUp, bool intakeUp) :
		CommandBase("PrepareForAutoCommand"), wedgesUp(wedgesUp), intakeUp(
				intakeUp) {
	Requires(wedgeSubsystem.get());
	Requires(intakeSubsystem.get());
}

void PrepareForAutoCommand::Initialize() {
	printf("PrepareForAuto: setting wedges %s | intake %s\n",
			wedgesUp ? "up" : "down", intakeUp ? "up" : "down");
	visionSubsystem->setVisionEnabled(true);
	SetTimeout(0.5); // give enough time for things to go into position
}

void PrepareForAutoCommand::Execute() {
	if (wedgesUp)
		wedgeSubsystem->liftWedge();
	else
		wedgeSubsystem->lowerWedge();

	if (intakeUp)
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
