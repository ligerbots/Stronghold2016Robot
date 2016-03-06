#include <Stronghold2016Robot.h>

DriveDistanceCommand::DriveDistanceCommand(double distance) :
		CommandBase("DriveDistanceCommand"), distance(distance), startPositionLeft(
				0), startPositionRight(0) {
	Requires(driveSubsystem.get());
}

void DriveDistanceCommand::Initialize() {
	startPositionLeft = driveSubsystem->getLeftEncoderPosition();
	startPositionRight = driveSubsystem->getRightEncoderPosition();
}

void DriveDistanceCommand::Execute() {
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
