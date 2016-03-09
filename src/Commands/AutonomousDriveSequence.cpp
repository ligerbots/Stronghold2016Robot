#include <Stronghold2016Robot.h>

AutonomousDriveSequence::AutonomousDriveSequence(int position, int defense, int target,
												 double orientation, bool slow) {
	// Add Commands here:
	// e.g. AddSequential(new Command1());
	//      AddSequential(new Command2());
	// these will run in order.

	// To run multiple commands at the same time,
	// use AddParallel()
	// e.g. AddParallel(new Command1());
	//      AddSequential(new Command2());
	// Command1 and Command2 will run in parallel.

	// A command group will require all of the subsystems that each member
	// would require.
	// e.g. if Command1 requires chassis, and Command2 requires arm,
	// a CommandGroup containing them would require both the chassis and the
	// arm.

	bool intakeUp = false;
	bool wedgesUp = false;

//	double driveSpeed = 0.8;

	AddSequential(new ToggleLedCommand(true));
	AddSequential(new ToggleCompressorCommand(true));
	AddSequential(new PrepareForAutoCommand(wedgesUp, intakeUp));
	AddSequential(new GearShiftCommand(GearShiftCommand::SHIFT_DOWN));

	double startX = FieldInfo::startingLocations[position].x;
	double startY = FieldInfo::startingLocations[position].y;

	double afterDefenseY = startY + FieldInfo::StartToDefenseDistance;

	double targetX = FieldInfo::targetLocations[target].x;
	double targetY = FieldInfo::targetLocations[target].y;

	double dx = targetX - startX;
	double dy = targetY - afterDefenseY;

	double firstAngle = atan2(dy, dx);
	double distanceToShootingPosition = sqrt(dx * dx + dy * dy);

	double secondAngle = FieldInfo::targetLineUpAngles[target];
	double drivedirection = orientation == 0.0 ? 1.0 : -1.0;

	AddSequential(new DriveDistanceCommand(drivedirection * FieldInfo::StartToDefenseDistance + FieldInfo::DrivePastDefense),
			slow ? DriveDistanceCommand::SLOW : DriveDistanceCommand::NORMAL);

	// NOTE!! Since our angles are absolute angles with respect to the field (as opposed to relative
	// angles with respect to the current position of the robot), the RotateIMUCommand below will
	// make the correct turn regardless of the initial orientation of the robot
	AddSequential(new RotateIMUCommand(90 - firstAngle)); // correct field angles to navx angles

	// Drive to the target spot in high gear, but let's leave it low speed for now
	AddSequential(new DriveDistanceCommand(distanceToShootingPosition,
			DriveDistanceCommand::SLOW, DriveDistanceCommand::HIGH));
	AddSequential(new RotateIMUCommand(secondAngle));
	// handle going to specified target

}
