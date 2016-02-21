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

}

bool DriveDistanceCommand::IsFinished() {
	return false;
}

void DriveDistanceCommand::End() {

}

void DriveDistanceCommand::Interrupted() {
}
