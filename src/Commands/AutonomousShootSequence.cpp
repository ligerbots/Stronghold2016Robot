#include <Stronghold2016Robot.h>

AutonomousShootSequence::AutonomousShootSequence()
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

	//TODO: switch back
//	AddSequential(new AcquireTarget(true));
	AddParallel(new RollBallAndIntakeUpSequence());
	AddSequential(new AcquireTarget(true));
	AddSequential(new CenterOnTargetCommand());
	AddSequential(new AcquireTarget(true));
	AddSequential(new WaitForIntakeUpCommand());
	AddSequential(new AutoSetFlapsCommand());
	AddSequential(new ShootCommand());
}

void AutonomousShootSequence::Interrupted(){
	CommandGroup::Interrupted();
}

void AutonomousShootSequence::End(){
	CommandGroup::End();
	cleanup();
}

void AutonomousShootSequence::cleanup(){
	if(DriverStation::GetInstance().IsOperatorControl() && GetGroup() == NULL){
		CommandBase::intakeRollerCommand->Start();
		CommandBase::driveJoystickCommand->Start();
		CommandBase::flapCommand->Start();
	}
}
