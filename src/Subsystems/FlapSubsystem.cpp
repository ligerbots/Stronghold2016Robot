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

bool FlapSubsystem::setFlapsDifferential(double distInches, double angle){
	if(distInches < 50.0){
		distInches = 50.0;
	}
	FieldInfo::VisionFlapDataPoint distLangleL = {0, 0, 0, 0};
	FieldInfo::VisionFlapDataPoint distLangleH = {0, 0, 0, 0};
	FieldInfo::VisionFlapDataPoint distHangleL = {0, 0, 0, 0};
	FieldInfo::VisionFlapDataPoint distHangleH = {0, 0, 0, 0};
	bool isDistLower = true;
	printf("Starting, %f %f\n", distInches, angle);
	for(unsigned int i = 0; i < sizeof(FieldInfo::differentialFlapData) / sizeof(FieldInfo::VisionFlapDataPoint); i++){
		printf("Loop %d\n", i);
		FieldInfo::VisionFlapDataPoint current = FieldInfo::differentialFlapData[i];
		printf("d: %f a %f\n", current.distance, current.fineAngle);

		if(current.distance > distInches){
			isDistLower = false;
		}

		printf("isDistLower %d\n", isDistLower);

		if(isDistLower){
			if(angle > current.fineAngle){
				distLangleL = current;
				printf("Setting dLaL\n");
			} else if(distLangleH.distance == 0){
				distLangleH = current;
				printf("Setting dLaH\n");
			}
		} else {
			if(angle > current.fineAngle){
				distHangleL = current;
				printf("Setting dHaL\n");
			} else if(distHangleH.distance == 0){
				distHangleH = current;
				printf("Setting dHaH\n");
				break;
			}
		}
	}
	if(distLangleL.distance == 0 ||
			distLangleH.distance == 0 ||
			distHangleL.distance == 0 ||
			distHangleH.distance == 0){
		printf("Flaps: can't calculate closest points!\n");
		return false;
	}

	double weightDistLangleL = calculateWeight(distLangleL, distInches, angle);
	double weightDistLangleH = calculateWeight(distLangleH, distInches, angle);
	double weightDistHangleL = calculateWeight(distHangleL, distInches, angle);
	double weightDistHangleH = calculateWeight(distHangleH, distInches, angle);

	double weightSum = weightDistLangleL + weightDistLangleH + weightDistHangleL + weightDistHangleH;

	double leftFlap =    (distLangleL.leftFlap * weightDistLangleL
						+ distLangleH.leftFlap * weightDistLangleH
						+ distHangleL.leftFlap * weightDistHangleL
						+ distHangleH.leftFlap * weightDistHangleH) / weightSum;

	double rightFlap =   (distLangleL.rightFlap * weightDistLangleL
						+ distLangleH.rightFlap * weightDistLangleH
						+ distHangleL.rightFlap * weightDistHangleL
						+ distHangleH.rightFlap * weightDistHangleH) / weightSum;

	setFlapsFraction(leftFlap, rightFlap);

	return true;
}

double FlapSubsystem::calculateWeight(FieldInfo::VisionFlapDataPoint point,
		double distInches, double angle){
	double normDiffAngle = (angle - point.fineAngle)
						 / (FieldInfo::flapDataMaxAngle - FieldInfo::flapDataMinAngle);

	double normDiffDist = (distInches - point.distance)
							 / (FieldInfo::flapDataMaxDistance - FieldInfo::flapDataMinDistance);

	return 1 / sqrt(normDiffAngle * normDiffAngle + normDiffDist * normDiffDist);
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
	if(Robot::ticks % 10) {
		return;
	}
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
