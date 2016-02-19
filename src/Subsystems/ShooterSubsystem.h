#pragma once

#include "WPILib.h"

class ShooterSubsystem: public Subsystem
{
private:
	std::unique_ptr<DoubleSolenoid> mp_shooterSolenoid;
public:
	ShooterSubsystem();
	void InitDefaultCommand();
	void firePistons();
	void retractPistons();
	void sendValuesToSmartDashboard();

};
