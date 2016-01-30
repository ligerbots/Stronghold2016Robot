#pragma once

#include "Commands/Subsystem.h"
#include "WPILib.h"

class Compressor: public Subsystem
{
private:
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities
public:
	Compressor();
	void InitDefaultCommand();
};

#endif
