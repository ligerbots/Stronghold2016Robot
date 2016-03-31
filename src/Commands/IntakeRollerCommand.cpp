#include <Stronghold2016Robot.h>

IntakeRollerCommand::IntakeRollerCommand() :
		CommandBase("IntakeRollerCommand")
{
	Requires(intakeSubsystem.get());
	SetInterruptible(true);
}

void IntakeRollerCommand::Initialize() {
}

void IntakeRollerCommand::Execute() {
	// range from -1 to 1
	OI *p_oi = Robot::instance->mp_operatorInterface;

	double speed = p_oi->mp_XboxController->GetRawAxis(3) - p_oi->mp_XboxController->GetRawAxis(2);

//	if (0 == Robot::ticks % 50) {
//		// every second check to see if the 2nd joystick appeared or disappeared
//		// This is to avoid endless messages if it's not plugged in, or plugged-in late
//		oi->registerSecondControllerButtons();
//	}
	if (p_oi->m_secondControllerButtonCount > 4) {
		if (p_oi->mp_FarmController->GetRawButton(4)) {
			speed = 1;
		} else if(p_oi->mp_FarmController->GetRawButton(5)) {
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
