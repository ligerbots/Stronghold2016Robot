#include <Stronghold2016Robot.h>

IntakeToggleCommand::IntakeToggleCommand() :
		CommandBase("IntakeToggleCommand_") {
	intakeDown = false;
	Requires(intakeSubsystem.get());
}

void IntakeToggleCommand::Initialize() {
	intakeDown = !intakeSubsystem->isIntakeArmUp();
	printf("IntakeCommand: init\n");
}

void IntakeToggleCommand::Execute() {
	if (/*intakeDown*/ intakeSubsystem->getIntakeArmValue() != DoubleSolenoid::kReverse) {
		intakeSubsystem -> setIntakeArmUp();
		printf("IntakeCommand: arm up\n");
	}
	else {
		intakeSubsystem -> setIntakeArmDown();
		printf("IntakeCommand: arm down\n");
	}

}

bool IntakeToggleCommand::IsFinished() {
//	return true when the command should terminate
	return true;
}

void IntakeToggleCommand::End() {
CommandBase::intakeRollerCommand->Start();printf("IntakeCommand: end\n");
}

void IntakeToggleCommand::Interrupted() {
	printf("IntakeCommand: interrupted\n");
}
