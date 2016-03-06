#include <Stronghold2016Robot.h>

DriveDistanceIMUCommand::DriveDistanceIMUCommand(double distanceMeters,
		double speed) :
		CommandBase(
				"DriveDistanceIMUCommand_" + std::to_string(distanceMeters)
						+ "_" + std::to_string(speed)), distanceMeters2(
				distanceMeters * distanceMeters), startX(0), startY(0), startZ(0), speed(
				speed) {
	Requires(driveSubsystem.get());
	SetInterruptible(false);
}

void DriveDistanceIMUCommand::Initialize() {
	printf("DriveDistanceIMUCommand: init\n");
	startX = navXSubsystem->getNavX()->GetDisplacementX();
	startY = navXSubsystem->getNavX()->GetDisplacementY();
	startY = navXSubsystem->getNavX()->GetDisplacementZ();
}

void DriveDistanceIMUCommand::Execute() {
	driveSubsystem->drive(speed, 0);
}

bool DriveDistanceIMUCommand::IsFinished() {
	float x = navXSubsystem->getNavX()->GetDisplacementX();
	float y = navXSubsystem->getNavX()->GetDisplacementY();
	float z = navXSubsystem->getNavX()->GetDisplacementZ();
	return (x - startX) * (x - startX) + (y - startY) * (y - startY) + (z - startZ) * (z - startZ)
			>= distanceMeters2;
}

void DriveDistanceIMUCommand::End() {
	printf("DriveDistanceIMUCommand: end\n");
	driveSubsystem->zeroMotors();
}

void DriveDistanceIMUCommand::Interrupted() {
	printf("DriveDistanceIMUCommand: interrupted\n");
	driveSubsystem->zeroMotors();
}
