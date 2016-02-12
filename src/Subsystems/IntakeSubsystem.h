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

	static constexpr double ROLLER_SPEED = 1;
public:
	IntakeSubsystem();
	void InitDefaultCommand();
	void rollIn();
	void rollOut();
	void rollStop();
	double rollerRevolutions();
	bool isIntakeClear();
	void intakeArmUp();
	void intakeArmDown();
	bool haveBall();

	void sendValuesToSmartDashboard();
};
