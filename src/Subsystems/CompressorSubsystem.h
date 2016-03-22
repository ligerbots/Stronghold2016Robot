#pragma once

#include "Commands/Subsystem.h"
#include "WPILib.h"

class CompressorSubsystem: public Subsystem
{
private:
	std::unique_ptr<Compressor> mp_compressor;
	std::unique_ptr<AnalogInput> mp_pressureSensor;
	bool m_Enable;
	bool m_haveSensor;
public:
	CompressorSubsystem();
	void InitDefaultCommand();

	double getPressurePSI();
	void setCompressor(bool on);
	bool isCompressorOn();
	bool isPressureSwitchTriggered();
	void toggleCompressor();
	void sendValuesToSmartDashboard();
};
