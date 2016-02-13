#pragma once

#include "Commands/Subsystem.h"
#include "WPILib.h"

class CompressorSubsystem: public Subsystem
{
private:
	std::unique_ptr<Compressor> mp_compressor;
public:
	CompressorSubsystem();
	void InitDefaultCommand();

	void SetCompressor(bool on);
	bool IsCompressorOn();
	bool IsPressureSwitchTriggered();
};
