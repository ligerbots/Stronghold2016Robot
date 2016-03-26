#include <Stronghold2016Robot.h>

constexpr double DriveDistanceCommand::speeds[];

DriveDistanceCommand::DriveDistanceCommand(double distance, FieldInfo::Speeds speed, GEAR gear) :
		CommandBase("DriveDistanceCommand"),
		driveStraightGain("Drive StraightGain"),
		m_distance(distance / 12),
		m_startPositionLeft(0),
		m_startPositionRight(0),
		m_startAngle(0),
		m_speed(FieldInfo::NORMAL),
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
	m_startAngle = navXSubsystem->GetYaw();
	m_speed = speeds[m_speedRequested];
	m_gear = m_gearRequested;

	printf("DriveDistance: moving %f feet\n", m_distance);

	// ignore top gear shift for distances less than 3 ft
	if (m_gear==HIGH) {
		printf("m_distance: %f, >2: %d\n", fabs(m_distance), fabs(m_distance) > 2.0);
		if (fabs(m_distance) > 2.0){
			printf("DriveDistance: going to high gear\n");
			driveSubsystem->shiftUp();
		} else {
			// force low gear and normal speed
			m_gear = LOW;
			m_speed = NORMAL_SPEED;
			driveSubsystem->shiftDown();
		}
	} else{
		driveSubsystem->shiftDown();
	}
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
		double remaining;
		// if m_distance is positive, m_startRightPosition is positive, right is positive and increasing
		// if m_distance is negative, m_startRightPosition is negative, right is negative and decreasing
		//  negative example: -400 - (-600 - -300), so -400 - -300 = 100
		//  positive example: 400 - (600 - 300) = 400 - 300 = 100
		remaining = m_distance - (right - m_startPositionRight) * TICKS_PER_FOOT;
		if (fabs(remaining) < 2.0) {
			printf("DriveDistance: Shifting down for last 2 feet\n");
			m_gear = LOW;
			m_speed = NORMAL_SPEED;
			driveSubsystem->shiftDown();
		}
	}

	double angle = navXSubsystem->GetYaw();
	double angleCorrection = (m_startAngle - angle) * driveStraightGain.get();
	//double err = left - right;
	//double turn = err * 0.001;
	driveSubsystem->drive(m_distance > 0 ? -m_speed : m_speed, angleCorrection);
}

bool DriveDistanceCommand::IsFinished() {
	//double left = driveSubsystem->getLeftEncoderPosition();
	double right = driveSubsystem->getRightEncoderPosition();
	if (m_distance > 0)
		return right >= m_startPositionRight + m_distance * TICKS_PER_FOOT;
	else
		return right <= m_startPositionRight + m_distance * TICKS_PER_FOOT;
}

void DriveDistanceCommand::End() {
	driveSubsystem->zeroMotors();
	if(DriverStation::GetInstance().IsOperatorControl() && this->GetGroup() == NULL) {
		CommandBase::driveJoystickCommand->Start();
	}
}

void DriveDistanceCommand::Interrupted() {
	driveSubsystem->zeroMotors();
}
