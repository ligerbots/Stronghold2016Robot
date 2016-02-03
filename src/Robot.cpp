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

	printf("Spamming i2c\n");
	for(int i=0;i<0xFF;i++){
		I2C i2c(I2C::kOnboard, i);

		printf("%d\n", i);

		uint8_t bytes[4];
		bytes[0] = 0;
		bytes[1] = 65;
		bytes[2] = 66;
		bytes[3] = 67;
		bool res = i2c.WriteBulk(bytes, 4);

		if(res) printf("= true\n");
		else printf("= false\n");
	}
	printf("Done\n");
}

void Robot::AlwaysPeriodic(){
	ticks++;

	// other stuff
	CommandBase::visionSubsystem->updateVision(ticks);
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

