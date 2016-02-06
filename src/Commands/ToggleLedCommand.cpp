#include <Stronghold2016Robot.h>

ToggleLedCommand::ToggleLedCommand(bool on) : CommandBase("ToggleLedCommand_" + on) {
	Requires(visionSubsystem.get());
	this->on = on;
}

void ToggleLedCommand::Initialize() {
}

void ToggleLedCommand::Execute() {
	visionSubsystem->setLedRingOn(on);
}

bool ToggleLedCommand::IsFinished() {
	return true;
}

void ToggleLedCommand::End() {
}

void ToggleLedCommand::Interrupted() {
}
