#include <Stronghold2016Robot.h>

WaitForIntakeUpCommand::WaitForIntakeUpCommand() {
	SetTimeout(2);
}

void WaitForIntakeUpCommand::Initialize() {
}

void WaitForIntakeUpCommand::Execute() {
}

bool WaitForIntakeUpCommand::IsFinished() {
	return IsTimedOut() || intakeSubsystem->isIntakeArmUp();
}

void WaitForIntakeUpCommand::End() {
}

void WaitForIntakeUpCommand::Interrupted() {
}
