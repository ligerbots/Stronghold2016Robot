#include <Stronghold2016Robot.h>

DriveDistanceCommand::DriveDistanceCommand(double distance) :
		CommandBase("DriveDistanceCommand"), distance(distance), startPositionLeft(
				0), startPositionRight(0),
				driveStraightGain("Drive StraightGain") {
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
	(void)left;	// avoid warnings until we finish this function
	(void)right;
}

bool DriveDistanceCommand::IsFinished() {
	return false;
}

void DriveDistanceCommand::End() {

}

void DriveDistanceCommand::Interrupted() {
}
