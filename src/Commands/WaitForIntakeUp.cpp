#include <Stronghold2016Robot.h>

WaitForIntakeUp::WaitForIntakeUp() {
	SetTimeout(2);
}

void WaitForIntakeUp::Initialize() {
}

void WaitForIntakeUp::Execute() {
}

bool WaitForIntakeUp::IsFinished() {
	return IsTimedOut() || intakeSubsystem->isIntakeArmUp();
}

void WaitForIntakeUp::End() {
}

void WaitForIntakeUp::Interrupted() {
}
