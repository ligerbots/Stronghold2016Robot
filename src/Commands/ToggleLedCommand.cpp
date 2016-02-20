#include <Stronghold2016Robot.h>

ToggleLedCommand::ToggleLedCommand() :
		CommandBase("ToggleLedCommand_") {
	Requires(visionSubsystem.get());
}

void ToggleLedCommand::Initialize() {
}

void ToggleLedCommand::Execute() {
	visionSubsystem->setLedRingOn(!visionSubsystem->isLedRingOn());
}

bool ToggleLedCommand::IsFinished() {
	return true;
}

void ToggleLedCommand::End() {
}

void ToggleLedCommand::Interrupted() {
}
