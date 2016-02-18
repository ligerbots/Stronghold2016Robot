#include <Stronghold2016Robot.h>

ToggleCompressorCommand::ToggleCompressorCommand() :
		CommandBase("ToggleCompressorCommand_") {
	Requires(compressorSubsystem.get());
//	Requires(yourSubsystemHere.get());
}

void ToggleCompressorCommand::Initialize() {
	compressorSubsystem->toggleCompressor();
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
