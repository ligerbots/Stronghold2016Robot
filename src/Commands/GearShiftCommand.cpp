#include <Stronghold2016Robot.h>

GearShiftCommand::GearShiftCommand(Direction direction) : CommandBase("GearShiftCommand"),
	m_direction(direction)
{
}

void GearShiftCommand::Initialize() {
}

void GearShiftCommand::Execute() {
	if (m_direction == TOGGLE)
	{
		if (driveSubsystem->isShiftedUp()){
			driveSubsystem->shiftDown();
		} else {
			driveSubsystem->shiftUp();
		}
	}
	else if (m_direction == SHIFT_DOWN) driveSubsystem->shiftDown();
	else if (m_direction == SHIFT_UP)  driveSubsystem->shiftUp();
}

bool GearShiftCommand::IsFinished() {
	return true;
}

void GearShiftCommand::End() {

}

void GearShiftCommand::Interrupted() {

}
