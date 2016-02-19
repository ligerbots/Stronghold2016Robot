#pragma once

#include "Commands/Subsystem.h"
#include "WPILib.h"

class FlapSubsystem: public Subsystem
{
private:
	std::unique_ptr<Servo> mp_leftFlap;
	std::unique_ptr<Servo> mp_rightFlap;
public:
	FlapSubsystem();
	void InitDefaultCommand();
	void sendValuesToSmartDashboard();

	void setLeftFlapAngle(double angle);
	void setRightFlapAngle(double angle);
	double getRightFlapAngle();
	double getLeftFlapAngle();
};
