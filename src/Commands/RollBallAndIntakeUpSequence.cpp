#include <Stronghold2016Robot.h>

RollBallAndIntakeUpSequence::RollBallAndIntakeUpSequence()
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

	AddSequential(new RollBallToIntakePositionCommand(RollBallToIntakePositionCommand::SHOOTING_POSITION));
	AddSequential(new IntakeToggleCommand(true));
}

void RollBallAndIntakeUpSequence::Interrupted(){
	CommandGroup::Interrupted();
}

void RollBallAndIntakeUpSequence::End(){
	CommandGroup::End();
	cleanup();
}

void RollBallAndIntakeUpSequence::cleanup(){
	if(DriverStation::GetInstance().IsOperatorControl() && this->GetGroup() == NULL){
		CommandBase::intakeRollerCommand->Start();
		CommandBase::driveJoystickCommand->Start();
	}
}
