#pragma once

#include "WPILib.h"

class ShooterSubsystem: public Subsystem
{
private:
	std::unique_ptr<DoubleSolenoid> mp_shooterSolenoid;
public:
	ShooterSubsystem();
	void InitDefaultCommand();
	void firePiston();
	void retractPiston();
	void sendValuesToSmartDashboard();

};
