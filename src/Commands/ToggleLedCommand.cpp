#include <Stronghold2016Robot.h>

ToggleLedCommand::ToggleLedCommand() :
		CommandBase("ToggleLedCommand_") {
	Requires(visionSubsystem.get());
	m_turnOn = false;
	m_toggle = true;
}

ToggleLedCommand::ToggleLedCommand(bool on) {
	m_toggle = false;
	m_turnOn = on;
}

void ToggleLedCommand::Initialize() {
	if (m_toggle) {
		m_turnOn = !visionSubsystem->isLedRingOn();
	}
}

void ToggleLedCommand::Execute() {
	printf("ToggleLedCommand: turning led %s\n", m_turnOn ? "on" : "off");
	visionSubsystem->setLedRingState(m_turnOn);
}

bool ToggleLedCommand::IsFinished() {
	return true;
}

void ToggleLedCommand::End() {
}

void ToggleLedCommand::Interrupted() {
}
