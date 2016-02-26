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

void FlapSubsystem::setFlapsFraction(double fraction) {
	// transform [0, 1] to [flapLow, flapHigh]
	double leftFlapPosition = (leftHighLimit.get() - leftLowLimit.get())
			* fraction + leftLowLimit.get();
	double rightFlapPosition = (rightHighLimit.get() - rightLowLimit.get())
			* fraction + rightLowLimit.get();
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
