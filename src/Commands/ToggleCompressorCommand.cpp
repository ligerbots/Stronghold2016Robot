#include <Stronghold2016Robot.h>

ToggleCompressorCommand::ToggleCompressorCommand() :
		CommandBase("ToggleCompressorCommand_") {
	Requires(compressorSubsystem.get());
	m_mode = 2;
}

ToggleCompressorCommand::ToggleCompressorCommand(bool on) {
	m_mode = on;
}

void ToggleCompressorCommand::Initialize() {
	if (m_mode == 2) {
		compressorSubsystem->toggleCompressor();
	} else {
		compressorSubsystem->setCompressor(m_mode);
	}
}

void ToggleCompressorCommand::Execute() {

}

bool ToggleCompressorCommand::IsFinished() {
	return true;
}

void ToggleCompressorCommand::End() {

}

void ToggleCompressorCommand::Interrupted() {

}
