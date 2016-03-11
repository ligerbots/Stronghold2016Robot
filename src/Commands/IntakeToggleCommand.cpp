#include <Stronghold2016Robot.h>

IntakeToggleCommand::IntakeToggleCommand() :
		CommandBase("IntakeToggleCommand_") {
	Requires(intakeSubsystem.get());
	mode = 2;
}

IntakeToggleCommand::IntakeToggleCommand(bool intakeUp) :
		CommandBase("IntakeToggleCommand_") {
	Requires(intakeSubsystem.get());
	mode = intakeUp;
}

void IntakeToggleCommand::Initialize() {
	printf("IntakeCommand: init\n");
}

void IntakeToggleCommand::Execute() {
	if (mode == 2) {
		if (intakeSubsystem->getIntakeArmValue() != DoubleSolenoid::kReverse) {
			intakeSubsystem->setIntakeArmUp();
			printf("IntakeCommand: arm up\n");
		} else {
			intakeSubsystem->setIntakeArmDown();
			printf("IntakeCommand: arm down\n");
		}
	} else {
		if (mode) {
			intakeSubsystem->setIntakeArmUp();
			printf("IntakeCommand: arm up\n");
		} else {
			intakeSubsystem->setIntakeArmDown();
			printf("IntakeCommand: arm down\n");
		}
	}

}

bool IntakeToggleCommand::IsFinished() {
//	return true when the command should terminate
	return true;
}

void IntakeToggleCommand::End() {
	if(DriverStation::GetInstance().IsOperatorControl() && this->GetGroup() == NULL)
		CommandBase::intakeRollerCommand->Start();
	printf("IntakeCommand: end\n");
}

void IntakeToggleCommand::Interrupted() {
	if(DriverStation::GetInstance().IsOperatorControl() && this->GetGroup() == NULL)
		CommandBase::intakeRollerCommand->Start();
	printf("IntakeCommand: interrupted\n");
}
