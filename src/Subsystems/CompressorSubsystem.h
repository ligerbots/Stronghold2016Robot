#pragma once

#include "Commands/Subsystem.h"
#include "WPILib.h"

class CompressorSubsystem: public Subsystem
{
private:
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities
public:
	CompressorSubsystem();
	void InitDefaultCommand();
};
