#include <Stronghold2016Robot.h>

DriveDistanceIMUCommand::DriveDistanceIMUCommand(double distanceMeters) :
		CommandBase("DriveDistanceIMUCommand"), distanceMeters(distanceMeters), start(0) {
	Requires(driveSubsystem.get());
	SetInterruptible(false);
}

void DriveDistanceIMUCommand::Initialize() {

}

void DriveDistanceIMUCommand::Execute() {

}

bool DriveDistanceIMUCommand::IsFinished() {
	return false;
}

void DriveDistanceIMUCommand::End() {

}

void DriveDistanceIMUCommand::Interrupted() {

}
