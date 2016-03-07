#include <Stronghold2016Robot.h>

DriveDistanceCommand::DriveDistanceCommand(double distance) :
		CommandBase("DriveDistanceCommand"),
		driveStraightGain("Drive StraightGain"),
		distance(distance),
		startPositionLeft(0),
		startPositionRight(0) {
	Requires(driveSubsystem.get());
}

void DriveDistanceCommand::Initialize() {
	startPositionLeft = driveSubsystem->getLeftEncoderPosition();
	startPositionRight = driveSubsystem->getRightEncoderPosition();
}

void DriveDistanceCommand::Execute() {
#pragma GCC diagnostic ignored "-Wunused-variable"
	// From 2014 drive straight.
	// Use the gyro to compute how far off straight we are and adjust the turn
	// value in arcade drive
	// m_gyro = Robot.driveTrain.getCurrentAngle() - m_gyro_start;
	// Robot.driveTrain.drive((-m_gyro*Robot.AUTONOMOUS_DRIVE_GAIN)/100.0, 0.7);
	double left = driveSubsystem->getLeftEncoderPosition();
	double right = driveSubsystem->getRightEncoderPosition();
	double err = left - right;
	double turn = err * 0.001;
	driveSubsystem->drive(-0.7, 0);
}

bool DriveDistanceCommand::IsFinished() {
	double left = driveSubsystem->getLeftEncoderPosition();
	double right = driveSubsystem->getRightEncoderPosition();
	return right >= startPositionRight + distance * TICKS_PER_FOOT;
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
