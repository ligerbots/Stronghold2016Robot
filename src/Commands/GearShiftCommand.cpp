#include <Stronghold2016Robot.h>

GearShiftCommand::GearShiftCommand() : CommandBase("GearShiftCommand"){
	isShiftedUp = false;
}

void GearShiftCommand::Initialize() {
	isShiftedUp = driveSubsystem->isShiftedUp();
}

void GearShiftCommand::Execute() {
	if(isShiftedUp){
		driveSubsystem->shiftDown();
	} else {
		driveSubsystem->shiftUp();
	}
}

bool GearShiftCommand::IsFinished() {
	return true;
}

void GearShiftCommand::End() {

}

void GearShiftCommand::Interrupted() {

}
