#include <Stronghold2016Robot.h>

FlapSubsystem::FlapSubsystem() :
		Subsystem("FlapSubsystem"), leftLowLimit("FlapLeftLowLimit"), leftHighLimit(
				"FlapLeftHighLimit"), rightLowLimit("FlapRightLowLimit"), rightHighLimit(
				"FlapRightHighLimit"), m_currentLeftPosition(0), m_currentRightPosition(0) {
	mp_leftFlap.reset(new Servo(RobotMap::PWM_SERVO_SHOOTER_LEFT));
	mp_rightFlap.reset(new Servo(RobotMap::PWM_SERVO_SHOOTER_RIGHT));
	m_robotTickAtFlapsDown = 0;
}

void FlapSubsystem::InitDefaultCommand() {

}

void FlapSubsystem::setFlapsFraction(double fractionBoth) {
	setFlapsFraction(fractionBoth, fractionBoth);
}

void FlapSubsystem::setFlapsFractionDifferential(double fractionBoth, double dFraction){
	// method 1
	if(dFraction > 0){ // lower "left" flap, shoot to the left
		setFlapsFraction(fractionBoth - fabs(dFraction), fractionBoth);
	} else { // // lower "right" flap, shoot to the right
		setFlapsFraction(fractionBoth, fractionBoth - fabs(dFraction));
	}

#if false
	// method 2
	if(dFraction > 0){
		setFlapsFraction(fractionBoth - fabs(dFraction), fractionBoth + fabs(dFraction));
	} else {
		setFlapsFraction(fractionBoth + fabs(dFraction), fractionBoth - fabs(dFraction));
	}
#endif
}

// takes parameters from 0 to 1, scales to angles for both flaps
void FlapSubsystem::setFlapsFraction(double fractionLeft,
		double fractionRight) {
	// don't break the servos; make sure we have valid values
	fractionLeft = fmax(fmin(fractionLeft, 1), 0);
	fractionRight = fmax(fmin(fractionRight, 1), 0);

	if(fractionLeft == 1.0 && fractionRight == 1.0 && (m_currentLeftPosition != 1.0 || m_currentRightPosition != 1.0)){
		m_robotTickAtFlapsDown = Robot::ticks;
	}

	m_currentLeftPosition = fractionLeft;
	m_currentRightPosition = fractionRight;

	// transform [0, 1] to [flapLow, flapHigh]
	double leftFlapPosition = (leftHighLimit.get() - leftLowLimit.get())
			* fractionLeft + leftLowLimit.get();
	double rightFlapPosition = (rightHighLimit.get() - rightLowLimit.get())
			* fractionRight + rightLowLimit.get();
	setLeftFlapAngle(leftFlapPosition);
	setRightFlapAngle(rightFlapPosition);
}

bool FlapSubsystem::isSafeToIntake(){
	// did we set both flaps down more than 20 ticks ago?
//	printf("FlapSubsystem: left %f right %f ticksAtDown %d ticks %d\n", currentLeftPosition,currentRightPosition, m_robotTickAtFlapsDown, Robot::ticks);
	return m_currentLeftPosition == 1.0 && m_currentRightPosition == 1.0 && (Robot::ticks - m_robotTickAtFlapsDown) > 20;
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

double FlapSubsystem::getLeftFlapFraction(){
	return m_currentLeftPosition;
}

double FlapSubsystem::getRightFlapFraction(){
	return m_currentRightPosition;
}

double FlapSubsystem::getLeftFlapAngle() {
	return mp_leftFlap->GetAngle();
}

double FlapSubsystem::getRightFlapAngle() {
	return mp_rightFlap->GetAngle();
}
