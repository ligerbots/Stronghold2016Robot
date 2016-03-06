#include <Stronghold2016Robot.h>

PrepareForCrossingSequence::PrepareForCrossingSequence()
{
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

	AddSequential(new IntakeToggleCommand(false));
	AddSequential(new WedgeToggleCommand(false));
	AddSequential(new DelayCommand(0.5)); // wait for intake & wedges
	AddSequential(new RollBallToIntakePositionCommand(RollBallToIntakePositionCommand::CROSSING_POSITION));
}

void PrepareForCrossingSequence::Initialize(){
	printf("PrepareForCrossingSequence: init\n");
	CommandGroup::Initialize();
}

void PrepareForCrossingSequence::Interrupted(){
	CommandGroup::Interrupted();
	Cleanup();
}

void PrepareForCrossingSequence::End(){
	CommandGroup::End();
	Cleanup();
}

void PrepareForCrossingSequence::Cleanup(){
	if(DriverStation::GetInstance().IsOperatorControl()){
		CommandBase::flapCommand->Start();
		CommandBase::intakeRollerCommand->Start();
	}
}
