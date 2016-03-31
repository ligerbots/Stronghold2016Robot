#include <Stronghold2016Robot.h>

WedgeToggleCommand::WedgeToggleCommand() :
		CommandBase("WedgeToggleCommand_") {
	Requires(wedgeSubsystem.get());
	m_action = 2; // toggle
}

WedgeToggleCommand::WedgeToggleCommand(bool wedgeUp) :
		CommandBase("WedgeToggleCommand_" + std::to_string(wedgeUp)) {
	Requires(wedgeSubsystem.get());
	m_action = wedgeUp;
}

void WedgeToggleCommand::Initialize() {
}

void WedgeToggleCommand::Execute() {
	bool lift;
	if (m_action == 2) {
		lift = wedgeSubsystem->getWedgeValue() != DoubleSolenoid::kReverse;
	} else {
		lift = m_action;
	}
	if (lift) {
		printf("WedgeToggleCommand: lifting wedges\n");
		wedgeSubsystem->liftWedge();
	} else {
		printf("WedgeToggleCommand: lowering wedges\n");
		wedgeSubsystem->lowerWedge();
	}
}

bool WedgeToggleCommand::IsFinished() {
	return true;
}

void WedgeToggleCommand::End() {

}

void WedgeToggleCommand::Interrupted() {

}
