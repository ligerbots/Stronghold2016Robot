#include <Stronghold2016Robot.h>

Robot* Robot::instance = NULL;

Robot::Robot() {
	instance = this;
	pAutonomousModeChooser = new SendableChooser();
	pOperatorInterface = new OI();
	ticks = 0;
}

Robot::~Robot(){
}

void Robot::RobotInit() {
	CommandBase::init();

	pAutonomousModeChooser->AddDefault("Default Auto", new ExampleCommand());
	SmartDashboard::PutData("Auto Modes", pAutonomousModeChooser);
}

void Robot::AlwaysPeriodic(){
	ticks++;

	// send camera images
	// other stuff
}

void Robot::DisabledInit() {
}

void Robot::DisabledPeriodic(){
	AlwaysPeriodic();
}

void Robot::AutonomousInit() {
	/* std::string autoSelected = SmartDashboard::GetString("Auto Selector", "Default");
	 if(autoSelected == "My Auto") {
	 autonomousCommand.reset(new MyAutoCommand());
	 } else {
	 autonomousCommand.reset(new ExampleCommand());
	 } */

}

void Robot::AutonomousPeriodic() {
	AlwaysPeriodic();
	Scheduler::GetInstance()->Run();
}

void Robot::TeleopInit() {
	// This makes sure that the autonomous stops running when
	// teleop starts running. If you want the autonomous to
	// continue until interrupted by another command, remove
	// this line or comment it out.
}

void Robot::TeleopPeriodic() {
	AlwaysPeriodic();
	Scheduler::GetInstance()->Run();
}

void Robot::TestPeriodic() {
	LiveWindow::GetInstance()->Run();
}

START_ROBOT_CLASS(Robot)

