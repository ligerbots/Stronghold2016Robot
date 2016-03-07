#include <Stronghold2016Robot.h>

WedgeToggleCommand::WedgeToggleCommand() :
		CommandBase("WedgeToggleCommand_") {
	Requires(wedgeSubsystem.get());
	action = 2; // toggle
}

WedgeToggleCommand::WedgeToggleCommand(bool wedgeUp) :
		CommandBase("WedgeToggleCommand_" + std::to_string(wedgeUp)) {
	Requires(wedgeSubsystem.get());
	action = wedgeUp;
}

void WedgeToggleCommand::Initialize() {
}

void WedgeToggleCommand::Execute() {
	bool lift;
	if (action == 2)
		lift = wedgeSubsystem->getWedgeValue() != DoubleSolenoid::kReverse;
	else
		lift = action;
	if (lift) {
		printf("WedgeToggleCommand: lifting wedges\n");
		wedgeSubsystem->liftWedge();
	} else {
		printf("WedgeToggleCommand: lowering wedges\n");
		wedgeSubsystem->lowerWedge();
	}
}

bool WedgeToggleCommand::IsFinished() {
//	return true when the command should terminate
	return true;
}

void WedgeToggleCommand::End() {

}

void WedgeToggleCommand::Interrupted() {

}
