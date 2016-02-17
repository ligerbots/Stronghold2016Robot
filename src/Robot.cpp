#include <Stronghold2016Robot.h>

Robot* Robot::instance = NULL;

Robot::Robot() {
	instance = this;
	mp_autonomousModeChooser = new SendableChooser();
	mp_operatorInterface = new OI();
	ticks = 0;
}

Robot::~Robot() {
}

void Robot::RobotInit() {
	CommandBase::init();

	mp_operatorInterface->registerCommands();

	CommandBase::visionSubsystem->camerasOn();

//	mp_autonomousModeChooser->AddDefault("Default Auto", new ExampleCommand());
	SmartDashboard::PutData("Auto Modes", mp_autonomousModeChooser);

//	CommandBase::compressorSubystem->SetCompressor(true);

	printf("Writing i2c\n");
	for (int i = 3; i < 4; i++) {
		I2C i2c(I2C::kOnboard, i);

		printf("%d\n", i);

		uint8_t bytes[30 * 3 + 2];
		bytes[0] = 0;
		for (int j = 1; j < 30 * 3 + 1; j += 3) {
			bytes[j] = j * 2;
			bytes[j + 1] = j * 2;
			bytes[j + 2] = j * 2;
		}
		bytes[30 * 3 + 1] = 254;
		i2c.WriteBulk(bytes, sizeof(bytes));
	}
	printf("Done\n");
}

void Robot::AlwaysPeriodic() {
	ticks++;

	// other stuff
	CommandBase::visionSubsystem->updateVision(ticks);

	CommandBase::navXSubsystem->sendValuesToSmartDashboard();
	CommandBase::driveSubsystem->sendValuesToSmartDashboard();
	// pdpSubsystem started spitting out errors. Debug this later - cbf 2016.02.15
	//CommandBase::pdpSubystem->sendValuesToSmartDashboard();

//	if (mp_operatorInterface->joystickButtonPressed(
//				mp_operatorInterface->pXboxController, 4)){
//		CommandBase::compressorSubystem->SetCompressor(!CommandBase::compressorSubystem->IsCompressorOn());
//	}

	if (mp_operatorInterface->joystickButtonPressed(
			mp_operatorInterface->pXboxController, 3)) {
		printf("Joystick 0:\n");
		printf("\tName: %s\n",
				DriverStation::GetInstance().GetJoystickName(0).c_str());
		printf("\tType: %d\n", DriverStation::GetInstance().GetJoystickType(0));
		printf("\tIsXbox: %d\n",
				DriverStation::GetInstance().GetJoystickIsXbox(0));
		printf("Joystick 1:\n");
		printf("\tName: %s\n",
				DriverStation::GetInstance().GetJoystickName(1).c_str());
		printf("\tType: %d\n", DriverStation::GetInstance().GetJoystickType(1));
		printf("\tIsXbox: %d\n",
				DriverStation::GetInstance().GetJoystickIsXbox(1));
	}
}

void Robot::DisabledInit() {
	printf("Robot: DisabledInit\n");
}

void Robot::DisabledPeriodic() {
	AlwaysPeriodic();
}

void Robot::AutonomousInit() {
	/* std::string autoSelected = SmartDashboard::GetString("Auto Selector", "Default");
	 if(autoSelected == "My Auto") {
	 autonomousCommand.reset(new MyAutoCommand());
	 } else {
	 autonomousCommand.reset(new ExampleCommand());
	 } */
	printf("Robot: AutononmousInit\n");

	CommandBase::driveJoystickCommand->Cancel();
}

void Robot::AutonomousPeriodic() {
	AlwaysPeriodic();
	Scheduler::GetInstance()->Run();
}

void Robot::TeleopInit() {
	printf("Robot: TeleopInit\n");
	// This makes sure that the autonomous stops running when
	// teleop starts running. If you want the autonomous to
	// continue until interrupted by another command, remove
	// this line or comment it out.
	CommandBase::driveJoystickCommand->Start();
}

void Robot::TeleopPeriodic() {
	AlwaysPeriodic();
	Scheduler::GetInstance()->Run();


}

void Robot::TestPeriodic() {
	LiveWindow::GetInstance()->Run();
}

START_ROBOT_CLASS(Robot)

