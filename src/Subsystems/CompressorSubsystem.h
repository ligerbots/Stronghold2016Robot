#pragma once

#include "Commands/Subsystem.h"
#include "WPILib.h"

class CompressorSubsystem: public Subsystem
{
private:
	std::unique_ptr<Compressor> mp_compressor;
	bool m_Enable;
public:
	CompressorSubsystem();
	void InitDefaultCommand();

	void setCompressor(bool on);
	bool isCompressorOn();
	bool isPressureSwitchTriggered();
	void toggleCompressor();
	void sendValuesToSmartDashboard();
};
