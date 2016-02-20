#include <Stronghold2016Robot.h>

GearShiftUpCommand::GearShiftUpCommand() :
		CommandBase("GearShiftUpCommand_") {
	Requires(driveSubsystem.get());
}

void GearShiftUpCommand::Initialize() {

}

void GearShiftUpCommand::Execute() {
	driveSubsystem -> shiftUp();
}

bool GearShiftUpCommand::IsFinished() {
//	return true when the command should terminate
	return true;
}

void GearShiftUpCommand::End() {

}

void GearShiftUpCommand::Interrupted() {

}
