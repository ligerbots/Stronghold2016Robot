#include <Stronghold2016Robot.h>

IntakeToggleCommand::IntakeToggleCommand() :
		CommandBase("IntakeToggleCommand_") {
	intakeDown = false;
	Requires(intakeSubsystem.get());
}

void IntakeToggleCommand::Initialize() {
	intakeDown = !intakeSubsystem->isIntakeArmUp();
}

void IntakeToggleCommand::Execute() {
	if (/*intakeDown*/ intakeSubsystem->getIntakeArmValue() != DoubleSolenoid::kReverse) {
		intakeSubsystem -> setIntakeArmUp();
	}
	else {
		intakeSubsystem -> setIntakeArmDown();
	}

}

bool IntakeToggleCommand::IsFinished() {
//	return true when the command should terminate
	return true;
}

void IntakeToggleCommand::End() {
CommandBase::intakeRollerCommand->Start();
}

void IntakeToggleCommand::Interrupted() {

}
