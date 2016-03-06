#include <Stronghold2016Robot.h>

IntakeRollerCommand::IntakeRollerCommand() :
		CommandBase("IntakeRollerCommand"),
		robot(Robot::instance)
{
	Requires(intakeSubsystem.get());
	SetInterruptible(true);
}

void IntakeRollerCommand::Initialize() {
}

void IntakeRollerCommand::Execute() {
	// range from -1 to 1
	OI *oi = robot->mp_operatorInterface;

	double speed = oi->pXboxController->GetRawAxis(3) - oi->pXboxController->GetRawAxis(2);

//	if (0 == Robot::ticks % 50) {
//		// every second check to see if the 2nd joystick appeared or disappeared
//		// This is to avoid endless messages if it's not plugged in, or plugged-in late
//		oi->registerSecondControllerButtons();
//	}
	if (oi->m_secondControllerPresent) {
		if (oi->pFarmController->GetRawButton(4)) {
			speed = 1;
		} else if(oi->pFarmController->GetRawButton(5)) {
			speed = -1;
		}
	}

	if (fabs(speed) > 0.1)
		intakeSubsystem->setRollSpeed(speed);
	else
		intakeSubsystem->rollStop();
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
