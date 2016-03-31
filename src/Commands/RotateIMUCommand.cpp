#include <Stronghold2016Robot.h>

RotateIMUCommand::RotateIMUCommand(double targetAngle, bool absolute) :
		CommandBase("RotateIMUCommand_" + std::to_string(targetAngle)), m_angle(targetAngle), m_targetAngle(0), m_currentAngle(
				0), m_lastAngle(0), m_isClockwise(false), m_isAbsolute(absolute) {
	Requires(driveSubsystem.get());
	SetInterruptible(false);
	m_ticks = 0;
}

void RotateIMUCommand::Initialize() {
	printf("RotateIMUCommand: init\n");

	driveSubsystem->shiftDown();

	updateCurrentAngle();
	if (m_isAbsolute) {
		m_targetAngle = m_angle;
	} else {
		m_targetAngle = m_currentAngle + m_angle;
	}
	printf("Current angle %f, angle %f, targetAngle %f, absolute %d\n", m_currentAngle, m_angle, m_targetAngle, m_isAbsolute);
	m_targetAngle = fmod(m_targetAngle + 360.0, 360.0);

	// find the shortest direction to turn
	if (m_targetAngle > m_currentAngle) {
		m_isClockwise = (m_targetAngle - m_currentAngle
				< m_currentAngle + (360 - m_targetAngle));
	} else {
		m_isClockwise = (m_currentAngle - m_targetAngle
				> m_targetAngle + (360 - m_currentAngle));
	}

	printf("Turn clockwise: %d\n", m_isClockwise);

	m_lastAngle = m_currentAngle;

	SetTimeout(5);
}

void RotateIMUCommand::updateCurrentAngle() {
	m_currentAngle = navXSubsystem->GetYaw();
	m_currentAngle = fmod(m_currentAngle + 360.0, 360.0); // turn -180,180 to 0,360
}

void RotateIMUCommand::Execute() {
	double speed = 1;
	m_ticks++;

	double error = fabs(m_currentAngle - m_targetAngle);
	if(error > 180){
		error = 360 - error; // we always try to go the minimum number of degrees
	}
	if(error < RAMP_DOWN_ZONE){ // ramp down as we approach target
		speed = error * (1 - MIN_SPEED) / RAMP_DOWN_ZONE + MIN_SPEED;
	} else if(m_ticks < RAMP_UP_TICKS){ // ramp up as we begin
		speed = MIN_SPEED + ((1 - MIN_SPEED) * m_ticks / RAMP_UP_TICKS);
	}

	driveSubsystem->drive(0, m_isClockwise ? speed : -speed);
	m_lastAngle = m_currentAngle;
	updateCurrentAngle();
}

bool RotateIMUCommand::IsFinished() {
	bool finished = false;
	// find if we passed the target angle (in mod 360)
	printf("Rotate: %f %f %f\n", m_lastAngle, m_targetAngle, m_currentAngle);
	if(m_lastAngle == m_currentAngle) return IsTimedOut(); // first iteration
//	if ((isClockwise && lastAngle < currentAngle) || (!isClockwise && currentAngle < lastAngle)) {
//		finished = isClockwise ? lastAngle <= targetAngle && targetAngle <= currentAngle
//				: lastAngle >= targetAngle && targetAngle >= currentAngle;
//	} else {
//		finished = !(isClockwise ? currentAngle <= targetAngle && targetAngle <= lastAngle
//				: currentAngle >= targetAngle && targetAngle >= lastAngle);
//	}

	// check "error" in both turn directions
	double error1 = fabs(m_targetAngle - m_currentAngle);
	double error2 = fabs(360 - error1);
	finished = error1 < 1.5 || error2 < 1.5; // if either is very small, we are on target

	return IsTimedOut() || finished;
}

void RotateIMUCommand::End() {
	printf("RotateIMUCommand: end\n");
	driveSubsystem->zeroMotors();
	if(DriverStation::GetInstance().IsOperatorControl() && this->GetGroup() == NULL){
		CommandBase::driveJoystickCommand->Start();
	}
}

void RotateIMUCommand::Interrupted() {
	printf("RotateIMUCommand: interrupted\n");
	driveSubsystem->zeroMotors();
	if(DriverStation::GetInstance().IsOperatorControl() && this->GetGroup() == NULL){
		CommandBase::driveJoystickCommand->Start();
	}
}
