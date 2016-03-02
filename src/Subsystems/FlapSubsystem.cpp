#include <Stronghold2016Robot.h>

FlapSubsystem::FlapSubsystem() :
		Subsystem("FlapSubsystem"), leftLowLimit("FlapLeftLowLimit"), leftHighLimit(
				"FlapLeftHighLimit"), rightLowLimit("FlapRightLowLimit"), rightHighLimit(
				"FlapRightHighLimit") {
	mp_leftFlap.reset(new Servo(RobotMap::PWM_SERVO_SHOOTER_LEFT));
	mp_rightFlap.reset(new Servo(RobotMap::PWM_SERVO_SHOOTER_RIGHT));
}

void FlapSubsystem::InitDefaultCommand() {

}

void FlapSubsystem::setFlapsFraction(double fractionBoth) {
	setFlapsFraction(fractionBoth, fractionBoth);
}

// not a member function
// if anything else needs a function like this, it should be moved to a utility file or something
static inline double constrain(double x, double high, double low) {
	if (x < low) {
		x = low;
	}
	if (x > high) {
		x = high;
	}
	return x;
}

void FlapSubsystem::setFlapsFraction(double fractionLeft,
		double fractionRight) {
	// don't break the servos; make sure we have valid values
	//fractionLeft = constrain(fractionLeft, 0, 1);
	//fractionRight = constrain(fractionRight, 0, 1);
	// transform [0, 1] to [flapLow, flapHigh]
	double leftFlapPosition = (leftHighLimit.get() - leftLowLimit.get())
			* fractionLeft + leftLowLimit.get();
	double rightFlapPosition = (rightHighLimit.get() - rightLowLimit.get())
			* fractionRight + rightLowLimit.get();
	setLeftFlapAngle(leftFlapPosition);
	setRightFlapAngle(rightFlapPosition);
}

void FlapSubsystem::sendValuesToSmartDashboard() {
	SmartDashboard::PutNumber("LeftFlapAngle", mp_leftFlap->GetAngle());
	SmartDashboard::PutNumber("RightFlapAngle", mp_rightFlap->GetAngle());
}

void FlapSubsystem::setLeftFlapAngle(double angle) {
	// servo is reversed, so flip the angle
	mp_leftFlap->SetAngle(180 - angle);
}

void FlapSubsystem::setRightFlapAngle(double angle) {
	mp_rightFlap->SetAngle(angle);
}

double FlapSubsystem::getLeftFlapAngle() {
	return mp_leftFlap->GetAngle();
}

double FlapSubsystem::getRightFlapAngle() {
	return mp_rightFlap->GetAngle();
}
