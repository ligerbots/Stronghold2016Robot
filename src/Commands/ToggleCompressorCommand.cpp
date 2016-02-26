#include <Stronghold2016Robot.h>

ToggleCompressorCommand::ToggleCompressorCommand() :
		CommandBase("ToggleCompressorCommand_") {
	Requires(compressorSubsystem.get());
	mode = 2;
}

ToggleCompressorCommand::ToggleCompressorCommand(bool on) {
	mode = on;
}

void ToggleCompressorCommand::Initialize() {
	if (mode == 2)
		compressorSubsystem->toggleCompressor();
	else
		compressorSubsystem->setCompressor(mode);
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
