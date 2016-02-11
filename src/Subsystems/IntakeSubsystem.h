#pragma once

#include "Commands/Subsystem.h"
#include "WPILib.h"

class IntakeSubsystem: public Subsystem
{
private:
	std::unique_ptr<DoubleSolenoid> mp_intakeArmSolenoid;
	std::unique_ptr<CANTalon> mp_rollers;
	std::unique_ptr<DigitalInput> mp_intakeUpSwitch;
	std::unique_ptr<DigitalInput> mp_intakeDownSwitch;
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
