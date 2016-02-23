#include <Stronghold2016Robot.h>

CompressorSubsystem::CompressorSubsystem() :
		Subsystem("Compressor") {
	// Since they ripped the PCM off of Road Kill, we'd better not reference it
	// It would be better if we could test for the presence/absence of the PCM
	// directly, but unfortunately that doesn't seem possible through WPILib
	m_Enable = !Robot::isRoadkill;
	if (m_Enable) mp_compressor.reset(new Compressor(RobotMap::PCM_CAN));
}

void CompressorSubsystem::InitDefaultCommand() {
}

void CompressorSubsystem::setCompressor(bool on) {
	if (m_Enable) mp_compressor->SetClosedLoopControl(on);
}

bool CompressorSubsystem::isCompressorOn() {
	return m_Enable && mp_compressor->GetClosedLoopControl();
}

bool CompressorSubsystem::isPressureSwitchTriggered() {
	return m_Enable && mp_compressor->GetPressureSwitchValue();
}

void CompressorSubsystem::toggleCompressor() {
	if (m_Enable) setCompressor(not (isCompressorOn()));
}

void CompressorSubsystem::sendValuesToSmartDashboard() {
	SmartDashboard::PutBoolean("Compressor/On", isCompressorOn());
	if (Robot::ticks % 50 == 12) // don't spam the can bus and stagger with other polling
		SmartDashboard::PutBoolean("Compressor/PressureSwitch",
				isPressureSwitchTriggered());
}
