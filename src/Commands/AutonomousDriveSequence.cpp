#include <Stronghold2016Robot.h>

AutonomousDriveSequence::AutonomousDriveSequence(int position, int defense, int target) {
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
	bool wedgesUp = FieldInfo::defenseStrategy[defense].wedgesUp;

//	double driveSpeed = 0.8;

	AddSequential(new ToggleLedCommand(true));
	AddSequential(new ToggleCompressorCommand(true));
	AddSequential(new PrepareForAutoCommand(wedgesUp, intakeUp));
	AddSequential(new GearShiftCommand(GearShiftCommand::SHIFT_DOWN));

	double startX = FieldInfo::startingLocations[position].x;
	double startY = FieldInfo::startingLocations[position].y;

	double afterDefenseY = startY + FieldInfo::StartToDefenseDistance + FieldInfo::DefenseDepth;

	double targetX = FieldInfo::targetLocations[target].x;
	double targetY = FieldInfo::targetLocations[target].y;

	double dx = targetX - startX;
	double dy = targetY - afterDefenseY;

	printf("AutoDrive: %f %f %f %f\n", targetX, startX, targetY, afterDefenseY);

	double firstAngle = atan2(dy, dx) * 180 / M_PI;
	double distanceToShootingPosition = sqrt(dx * dx + dy * dy);
	printf("%f\n", distanceToShootingPosition);

	double secondAngle = FieldInfo::targetLineUpAngles[target];
	double orientation = FieldInfo::defenseStrategy[defense].Orientation;
	FieldInfo::Speeds speed = FieldInfo::defenseStrategy[defense].speed;
	double driveDirection = orientation != 0.0 ? 1.0 : -1.0;

	if (!speed==FieldInfo::NOGO) {
		if(defense == FieldInfo::DEF_LOW_BAR || defense == FieldInfo::DEF_PORTCULLIS){
			AddParallel(new RollBallToIntakePositionCommand(RollBallToIntakePositionCommand::CROSSING_POSITION));
		}
		AddSequential(new DriveDistanceCommand(
				driveDirection * (FieldInfo::StartToDefenseDistance + FieldInfo::DefenseDepth + FieldInfo::DrivePastDefense),
				speed, DriveDistanceCommand::LOW));

		AddSequential(new WedgeToggleCommand(true));

		AddSequential(new DelayCommand(0.1));

		// NOTE!! Since our angles are absolute angles with respect to the field (as opposed to relative
		// angles with respect to the current position of the robot), the RotateIMUCommand below will
		// make the correct turn regardless of the initial orientation of the robot
		AddSequential(new RotateIMUCommand(90 - firstAngle)); // correct field angles to navx angles

		AddParallel(new RollBallToIntakePositionCommand(RollBallToIntakePositionCommand::SHOOTING_POSITION));

		AddSequential(new DelayCommand(0.1));

		// Drive to the target spot in high gear, but let's leave it low speed for now
		AddSequential(new DriveDistanceCommand(-distanceToShootingPosition,
				FieldInfo::FAST, DriveDistanceCommand::HIGH));
		AddSequential(new DelayCommand(0.1));
		AddSequential(new RotateIMUCommand(secondAngle));
		AddSequential(new DelayCommand(0.1));
		// handle going to specified target
	}

}

void AutonomousDriveSequence::End(){
	CommandGroup::End();
	cleanup();
}

void AutonomousDriveSequence::Interrupted(){
	CommandGroup::Interrupted();
	cleanup();
}

void AutonomousDriveSequence::cleanup(){
	if(DriverStation::GetInstance().IsOperatorControl()){
		CommandBase::driveJoystickCommand->Start();
	}
}
