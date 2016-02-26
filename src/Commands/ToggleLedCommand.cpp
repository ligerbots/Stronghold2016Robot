#include <Stronghold2016Robot.h>

ToggleLedCommand::ToggleLedCommand() :
		CommandBase("ToggleLedCommand_") {
	Requires(visionSubsystem.get());
	turnOn = false;
	toggle = true;
}

ToggleLedCommand::ToggleLedCommand(bool on) {
	toggle = false;
	turnOn = on;
}

void ToggleLedCommand::Initialize() {
	if (toggle)
		turnOn = !visionSubsystem->isLedRingOn();
}

void ToggleLedCommand::Execute() {
	printf("ToggleLedCommand: turning led %s\n", turnOn ? "on" : "off");
	visionSubsystem->setLedRingOn(turnOn);
}

bool ToggleLedCommand::IsFinished() {
	return true;
}

void ToggleLedCommand::End() {
}

void ToggleLedCommand::Interrupted() {
}
