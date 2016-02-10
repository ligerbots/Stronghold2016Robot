#include <Stronghold2016Robot.h>

CompressorSubsystem::CompressorSubsystem() :
		Subsystem("Compressor") {
	mp_compressor.reset(new Compressor(RobotMap::PCM_CAN));
}

void CompressorSubsystem::InitDefaultCommand() {
}

void CompressorSubsystem::SetCompressor(bool on){
	mp_compressor->SetClosedLoopControl(on);
}
