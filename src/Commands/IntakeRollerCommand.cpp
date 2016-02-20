#include <Stronghold2016Robot.h>

IntakeRollerCommand::IntakeRollerCommand() :
		CommandBase("IntakeRollerCommand") {
	Requires(intakeSubsystem.get());
	SetInterruptible(true);
}

void IntakeRollerCommand::Initialize() {
}

void IntakeRollerCommand::Execute() {
	// range from -1 to 1
	double speed =
			Robot::instance->mp_operatorInterface->pXboxController->GetRawAxis(3) -
			Robot::instance->mp_operatorInterface->pXboxController->GetRawAxis(2);
	intakeSubsystem->setRollSpeed(speed);
}

bool IntakeRollerCommand::IsFinished() {
	return false;
}

void IntakeRollerCommand::End() {
	intakeSubsystem->rollStop();
}

void IntakeRollerCommand::Interrupted() {
	intakeSubsystem->rollStop();
}
