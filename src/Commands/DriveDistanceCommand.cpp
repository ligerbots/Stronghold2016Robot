#include <Stronghold2016Robot.h>

DriveDistanceCommand::DriveDistanceCommand(double distance, SPEED speed, GEAR gear) :
		CommandBase("DriveDistanceCommand"),
		driveStraightGain("Drive StraightGain"),
		m_distance(distance),
		m_startPositionLeft(0),
		m_startPositionRight(0),
		m_startAngle(0),
		m_speed(NORMAL),
		m_gear(LOW),
		m_speedRequested(speed),
		m_gearRequested(gear)
{
	Requires(driveSubsystem.get());
	Requires(navXSubsystem.get());
}

void DriveDistanceCommand::Initialize() {
	m_startPositionLeft = driveSubsystem->getLeftEncoderPosition();
	m_startPositionRight = driveSubsystem->getRightEncoderPosition();
	m_startAngle = navXSubsystem->getYaw();
	m_speed = m_speedRequested==NORMAL ? NORMAL_SPEED : SLOW_SPEED;
	m_gear = m_gearRequested;

	// ignore top gear shift for distances less than 3 ft
	if (m_gear==HIGH) {
		if (m_distance > 2.0) driveSubsystem->shiftUp();
		else {
			// force low gear and normal speed
			m_gear = LOW;
			m_speed = NORMAL_SPEED;
			driveSubsystem->shiftDown();
		}
	}
	else driveSubsystem->shiftDown();
}

void DriveDistanceCommand::Execute() {
//#pragma GCC diagnostic ignored "-Wunused-variable"
	// From 2014 drive straight.
	// Use the gyro to compute how far off straight we are and adjust the turn
	// value in arcade drive
	// m_gyro = Robot.driveTrain.getCurrentAngle() - m_gyro_start;
	// Robot.driveTrain.drive((-m_gyro*Robot.AUTONOMOUS_DRIVE_GAIN)/100.0, 0.7);
	//double left = driveSubsystem->getLeftEncoderPosition();
	double right = driveSubsystem->getRightEncoderPosition();

	// if we're in high gear, expect some overshoot and ramp down
	if (m_gear==HIGH) {
		double remaining = (m_distance - right) * TICKS_PER_FOOT;
		if (remaining < 2.0) {
			m_gear = LOW;
			m_speed = NORMAL_SPEED;
			driveSubsystem->shiftDown();
		}
	}

	double angle = navXSubsystem->getYaw();
	double angleCorrection = - (m_startAngle - angle) * DRIVE_GAIN;
	//double err = left - right;
	//double turn = err * 0.001;
	driveSubsystem->drive(-m_speed, angleCorrection);
}

bool DriveDistanceCommand::IsFinished() {
	//double left = driveSubsystem->getLeftEncoderPosition();
	double right = driveSubsystem->getRightEncoderPosition();
	return right >= m_startPositionRight + m_distance * TICKS_PER_FOOT;
}

void DriveDistanceCommand::End() {
	driveSubsystem->zeroMotors();
	if(DriverStation::GetInstance().IsOperatorControl()){
		CommandBase::driveJoystickCommand->Start();
	}
}

void DriveDistanceCommand::Interrupted() {
	driveSubsystem->zeroMotors();
	if(DriverStation::GetInstance().IsOperatorControl()){
		CommandBase::driveJoystickCommand->Start();
	}
}
