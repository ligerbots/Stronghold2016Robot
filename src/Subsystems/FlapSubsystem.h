#pragma once

#include "Commands/Subsystem.h"
#include "WPILib.h"

class FlapSubsystem: public Subsystem {
private:
	std::unique_ptr<Servo> mp_leftFlap;
	std::unique_ptr<Servo> mp_rightFlap;

	Parameter<double> leftLowLimit;
	Parameter<double> leftHighLimit;
	Parameter<double> rightLowLimit;
	Parameter<double> rightHighLimit;
public:
	FlapSubsystem();
	void InitDefaultCommand();
	void sendValuesToSmartDashboard();

	void setLeftFlapAngle(double angle);
	void setRightFlapAngle(double angle);

	/**
	 * Sets both flaps according to min and max parameters.
	 * fraction=0 is all the way in one direction, and fraction=1
	 * is all the way in the other direction.
	 * @param fraction 0 to 1
	 */
	void setFlapsFraction(double fraction);

	double getRightFlapAngle();
	double getLeftFlapAngle();
};
