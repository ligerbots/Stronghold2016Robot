#include <Stronghold2016Robot.h>

DriveDistanceIMUCommand::DriveDistanceIMUCommand(double distanceMeters) :
		CommandBase("DriveDistanceIMUCommand"), distanceMeters2(
				distanceMeters * distanceMeters), startX(0), startY(0) {
	Requires(driveSubsystem.get());
	SetInterruptible(false);
}

void DriveDistanceIMUCommand::Initialize() {
	printf("DriveDistanceIMUCommand: init\n");
	startX = navXSubsystem->getNavX()->GetDisplacementX();
	startY = navXSubsystem->getNavX()->GetDisplacementY();
}

void DriveDistanceIMUCommand::Execute() {
	driveSubsystem->drive(1, 0);
}

bool DriveDistanceIMUCommand::IsFinished() {
	double x = navXSubsystem->getNavX()->GetDisplacementX();
	double y = navXSubsystem->getNavX()->GetDisplacementY();
	return (x - startX) * (x - startX) + (y - startY) * (y - startY) >= distanceMeters2;
}

void DriveDistanceIMUCommand::End() {
	printf("DriveDistanceIMUCommand: end\n");
	driveSubsystem->zeroMotors();
}

void DriveDistanceIMUCommand::Interrupted() {
	printf("DriveDistanceIMUCommand: interrupted\n");
	driveSubsystem->zeroMotors();
}