#pragma once

#include "Commands/Subsystem.h"
#include "WPILib.h"

class WedgeSubsystem: public Subsystem
{
private:
	std::unique_ptr<DoubleSolenoid> mp_wedgeArmSolenoid;
	std::unique_ptr<DigitalInput> mp_wedgeDownSwitch;
public:
	WedgeSubsystem();
	void InitDefaultCommand();
	void liftWedge();
	void lowerWedge();
	bool isWedgeDown();
	DoubleSolenoid::Value getWedgeValue();
	void sendValuesToSmartDashboard();
};

