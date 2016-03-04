#include <Stronghold2016Robot.h>

RotateIMUCommand::RotateIMUCommand(double targetAngle) :
		CommandBase("RotateIMUCommand_" + std::to_string(targetAngle)), targetAngle(targetAngle), currentAngle(
				0), lastAngle(0), isClockwise(false) {
	Requires(driveSubsystem.get());
	SetInterruptible(false);
}

void RotateIMUCommand::Initialize() {
	printf("RotateIMUCommand: init\n");

	updateCurrentAngle();
	targetAngle = fmod(targetAngle + 360.0, 360.0);

	// find the shortest direction to turn
	if (targetAngle > currentAngle) {
		isClockwise = (targetAngle - currentAngle
				> currentAngle + 360 - targetAngle);
	} else {
		isClockwise = (currentAngle - targetAngle
				< targetAngle + 360 - currentAngle);
	}

	lastAngle = currentAngle;

	SetTimeout(5);
}

void RotateIMUCommand::updateCurrentAngle() {
	currentAngle = navXSubsystem->getNavX()->GetYaw();
	currentAngle = fmod(currentAngle + 360.0, 360.0); // turn -180,180 to 0,360
}

void RotateIMUCommand::Execute() {
	driveSubsystem->drive(0, isClockwise ? 1 : -1);
	lastAngle = currentAngle;
	updateCurrentAngle();
}

bool RotateIMUCommand::IsFinished() {
	bool finished = false;
	// find if we passed the target angle (in mod 360)
	if ((isClockwise && lastAngle < currentAngle) || (!isClockwise && currentAngle < lastAngle)) {
		finished = lastAngle <= targetAngle && targetAngle <= currentAngle;
	} else {
		finished = !(currentAngle <= targetAngle && targetAngle <= lastAngle);
	}
	return IsTimedOut() || finished;
}

void RotateIMUCommand::End() {
	printf("RotateIMUCommand: end\n");
	driveSubsystem->zeroMotors();
}

void RotateIMUCommand::Interrupted() {
	printf("RotateIMUCommand: interrupted\n");
	driveSubsystem->zeroMotors();
}
