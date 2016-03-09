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
	startX = navXSubsystem->GetDisplacementX();
	startY = navXSubsystem->GetDisplacementY();
	startY = navXSubsystem->GetDisplacementZ();
}

void DriveDistanceIMUCommand::Execute() {
	driveSubsystem->drive(speed, 0);
}

bool DriveDistanceIMUCommand::IsFinished() {
	float x = navXSubsystem->GetDisplacementX();
	float y = navXSubsystem->GetDisplacementY();
	float z = navXSubsystem->GetDisplacementZ();
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
