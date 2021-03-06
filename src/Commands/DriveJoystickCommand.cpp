#include <Stronghold2016Robot.h>

double DriveJoystickCommand::TURN_MAX = 0.7;
double DriveJoystickCommand::LINEAR_MAX = 0.6;

DriveJoystickCommand::DriveJoystickCommand() : CommandBase("DriveJoystickCommand") {
	printf("DriveJoystickCommand: constructor\n");
	Requires(driveSubsystem.get());
}

void DriveJoystickCommand::Initialize() {
	printf("DriveJoystickCommand: initialize\n");
	// DriveSubsystem should have set everything correctly
	SetInterruptible(true);
}

void DriveJoystickCommand::Execute() {
	Joystick* p_XboxController =
			Robot::instance->mp_operatorInterface->mp_XboxController;
	Joystick* p_FarmController = Robot::instance->mp_operatorInterface->mp_FarmController;

	//double y = pXboxController->GetY();
	// there was a 0.7 turnMax variable in the 2015 bot
	//y = clampJoystickValue(y, -TURN_MAX, TURN_MAX);
	//double x = pXboxController->GetX();
	// roadkill safety
	//x = clampJoystickValue(x, -LINEAR_MAX, LINEAR_MAX);

	//SmartDashboard::PutNumber("DriveJoystick_x", x);
	//SmartDashboard::PutNumber("DriveJoystick_y", y);

	driveSubsystem->drive(p_XboxController->GetRawAxis(1) + p_FarmController->GetRawAxis(4),
			p_XboxController->GetRawAxis(4)+ p_FarmController->GetRawAxis(5));
}

bool DriveJoystickCommand::IsFinished() {
	// we always want joystick control
	return false;
}

void DriveJoystickCommand::End() {
	printf("DriveJoystickCommand: end");
}

void DriveJoystickCommand::Interrupted() {
	printf("DriveJoystickCommand: interrupted\n");
	driveSubsystem->zeroMotors();
}

double DriveJoystickCommand::clampJoystickValue(double value, double min,
		double max) {
	if (value < min)
		value = min;
	if (value > max)
		value = max;
	return value;
}
