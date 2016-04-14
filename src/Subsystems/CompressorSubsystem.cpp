#include <Stronghold2016Robot.h>

CompressorSubsystem::CompressorSubsystem() :
		Subsystem("Compressor") {
	// Since they ripped the PCM off of Road Kill, we'd better not reference it
	// It would be better if we could test for the presence/absence of the PCM
	// directly, but unfortunately that doesn't seem possible through WPILib
	m_Enable = !Robot::is_roadkill;
	if (m_Enable){
		mp_compressor.reset(new Compressor(RobotMap::PCM_CAN));
	}
	m_haveSensor = false;
	mp_pressureSensor.reset(new AnalogInput(RobotMap::AI_PRESSURE_SENSOR));
	double reading = mp_pressureSensor->GetVoltage();
	// if the sensor exists, it should be giving a nonzero input
	if(mp_pressureSensor->GetError().GetCode() == 0 && reading > 0){
		m_haveSensor = true;
	}
}

void CompressorSubsystem::InitDefaultCommand() {
}

double CompressorSubsystem::getPressurePSI(){
	if(!m_haveSensor){
		return -1;
	}
	double reading = mp_pressureSensor->GetVoltage();
	// formula from datasheet:
	// http://www.revrobotics.com/wp-content/uploads/2015/11/REV-11-1107-DS-00.pdf
	// Vcc assumed to be 5V
	double psi = 250 * (reading / 5) - 25;
	return psi;
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
	if(Robot::ticks % 10) {
		return;
	}
	SmartDashboard::PutBoolean("Compressor/On", isCompressorOn());
	if(Robot::ticks % 50 == 0){
		SmartDashboard::PutBoolean("Compressor/PressureSwitch",
				isPressureSwitchTriggered());
	}

//	SmartDashboard::PutNumber("Pneumatics_PSI", getPressurePSI());
}
