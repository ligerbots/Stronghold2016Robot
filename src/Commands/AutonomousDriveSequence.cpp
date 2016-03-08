#include <Stronghold2016Robot.h>

AutonomousDriveSequence::AutonomousDriveSequence(int position, int defense,
		int target) {
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

//	double startX = Robot::startingLocations[position].x;
//	double startY = Robot::startingLocations[position].y;
//
//	double afterDefenseX = startX + Robot::StartToDefenseDistance;
//
//	double targetX = Robot::targetLocations[target].x;
//	double targetY = Robot::targetLocations[target].y;

	AddSequential(new DriveDistanceCommand(Robot::StartToDefenseDistance + Robot::DrivePastDefense));
	AddSequential(new RotateIMUCommand(180));
	// handle going to specified target
}
