#include <Stronghold2016Robot.h>

CompressorSubsystem::CompressorSubsystem() :
		Subsystem("Compressor") {
	mp_compressor.reset(new Compressor(RobotMap::PCM_CAN));
}

void CompressorSubsystem::InitDefaultCommand() {
}

void CompressorSubsystem::setCompressor(bool on) {
	mp_compressor->SetClosedLoopControl(on);
}

bool CompressorSubsystem::isCompressorOn() {
	return mp_compressor->GetClosedLoopControl();
}

bool CompressorSubsystem::isPressureSwitchTriggered() {
	return mp_compressor->GetPressureSwitchValue();
}

void CompressorSubsystem::toggleCompressor() {
	setCompressor(not (isCompressorOn()));
}

void CompressorSubsystem::sendValuesToSmartDashboard() {
	SmartDashboard::PutBoolean("Compressor/On", isCompressorOn());
	if (Robot::ticks % 50 == 12) // don't spam the can bus and stagger with other polling
		SmartDashboard::PutBoolean("Compressor/PressureSwitch",
				isPressureSwitchTriggered());
}
