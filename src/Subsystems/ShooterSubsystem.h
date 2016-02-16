#pragma once

#include "Commands/Subsystem.h"
#include "WPILib.h"

class ShooterSubsystem: public Subsystem
{
private:
	std::unique_ptr<DoubleSolenoid> mp_shooterSolenoid;
	std::unique_ptr<Servo> mp_leftFlap;
	std::unique_ptr<Servo> mp_rightFlap;
public:
	ShooterSubsystem();
	void InitDefaultCommand();
	void shoot();
	void sendValuesToSmartDashboard();

	void setLeftFlap(double extent);
	void setRightFlap(double extent);
	void setFlaps(double extent);
	double getRightPosition();
	double getLeftPosition();
};
