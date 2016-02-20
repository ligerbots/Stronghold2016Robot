#include <Stronghold2016Robot.h>

GearShiftDownCommand::GearShiftDownCommand() :
		CommandBase("GearShiftDownCommand_") {
//	Requires(driveSubsystem.get());
}

void GearShiftDownCommand::Initialize() {

}

void GearShiftDownCommand::Execute() {
	driveSubsystem -> shiftDown();
}

bool GearShiftDownCommand::IsFinished() {
//	return true when the command should terminate
	return true;
}

void GearShiftDownCommand::End() {

}

void GearShiftDownCommand::Interrupted() {

}
