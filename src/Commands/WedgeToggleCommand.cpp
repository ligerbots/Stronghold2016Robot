#include <Stronghold2016Robot.h>

WedgeToggleCommand::WedgeToggleCommand() :
		CommandBase("WedgeToggleCommand_") {
	wedgeDown = false;
	Requires(wedgeSubsystem.get());
}

void WedgeToggleCommand::Initialize() {
	wedgeDown = wedgeSubsystem -> isWedgeDown();
}

void WedgeToggleCommand::Execute() {
	if (wedgeDown) {
		wedgeSubsystem -> liftWedge();
	}
	else {
		wedgeSubsystem -> lowerWedge();
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
