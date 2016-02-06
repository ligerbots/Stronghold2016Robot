#pragma once

#include "Commands/Subsystem.h"
#include "WPILib.h"

class IntakeSubsystem: public Subsystem
{
private:
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities
public:
	IntakeSubsystem();
	void InitDefaultCommand();
	void rollIn();
	void rollOut();
	bool intakeClear();
	void rollerUp();
	void rollerDown();
	double rollerRevolutions();
	bool haveBall();
};
