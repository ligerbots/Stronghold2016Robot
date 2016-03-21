#include <Stronghold2016Robot.h>
#include "RotateToTarget.h"

RotateToTarget::RotateToTarget() :
		CommandBase("RotateToTarget")
{
	Requires(driveSubsystem.get());
	SetInterruptible(true);
	m_ticks = 0;
}

void RotateToTarget::Initialize() {
	// Get the target angle from Vision. Will be zero if we have no target.
	double visionAngle = visionSubsystem->TargetAngle();
	targetAngle = (90 + visionAngle) + navXSubsystem->GetYaw();

	printf("RotateToTarget: vision angle %5.2f turn angle: %5.2f\n", visionAngle, targetAngle);

	driveSubsystem->shiftDown();

	updateCurrentAngle();
	if (isAbsolute)
		targetAngle = angle;
	else
		targetAngle = currentAngle + angle;
	targetAngle = fmod(targetAngle + 360.0, 360.0);

	// find the shortest direction to turn
	if (targetAngle > currentAngle) {
		isClockwise = (targetAngle - currentAngle
				< currentAngle + 360 - targetAngle);
	} else {
		isClockwise = (currentAngle - targetAngle
				> targetAngle + 360 - currentAngle);
	}

	printf("Turn clockwise: %d\n", isClockwise);

	lastAngle = currentAngle;

	SetTimeout(5);
}

void RotateToTarget::updateCurrentAngle() {
	currentAngle = navXSubsystem->GetYaw();
	currentAngle = fmod(currentAngle + 360.0, 360.0); // turn -180,180 to 0,360
}

void RotateToTarget::Execute() {
	double speed = 1;
	m_ticks++;

	double error = fabs(currentAngle - targetAngle);
	if(error > 180) error = 360 - error; // we always try to go the minimum number of degrees
	if(error < 30){
		speed = error * (1 - .6) / 30 + .6;
	} else if(m_ticks < 20){
		speed = .45 + (.55 * m_ticks / 20.0);
	}

	driveSubsystem->drive(0, isClockwise ? speed : -speed);
	lastAngle = currentAngle;
	updateCurrentAngle();
}

bool RotateToTarget::IsFinished() {
	bool finished = false;
	// find if we passed the target angle (in mod 360)
	printf("Rotate: %f %f %f\n", lastAngle, targetAngle, currentAngle);
	if(lastAngle == currentAngle) return IsTimedOut(); // first iteration
//	if ((isClockwise && lastAngle < currentAngle) || (!isClockwise && currentAngle < lastAngle)) {
//		finished = isClockwise ? lastAngle <= targetAngle && targetAngle <= currentAngle
//				: lastAngle >= targetAngle && targetAngle >= currentAngle;
//	} else {
//		finished = !(isClockwise ? currentAngle <= targetAngle && targetAngle <= lastAngle
//				: currentAngle >= targetAngle && targetAngle >= lastAngle);
//	}

	double error1 = fabs(targetAngle - currentAngle);
	double error2 = fabs(360 - error1);
	finished = error1 < 1.5 || error2 < 1.5;

	return IsTimedOut() || finished;
}

void RotateToTarget::End() {
	printf("RotateToTarget: end\n");
	driveSubsystem->zeroMotors();
	if (DriverStation::GetInstance().IsOperatorControl() && this->GetGroup() == NULL){
		CommandBase::driveJoystickCommand->Start();
	}
}

void RotateToTarget::Interrupted() {
	printf("RotateToTarget: interrupted\n");
	driveSubsystem->zeroMotors();
	if(DriverStation::GetInstance().IsOperatorControl() && this->GetGroup() == NULL){
		CommandBase::driveJoystickCommand->Start();
	}
}
