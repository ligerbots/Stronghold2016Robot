#include <Stronghold2016Robot.h>

// Robot wide globals whose definitions live in the Robot class
Robot* Robot::instance = NULL;
int Robot::ticks;
bool Robot::isRoadkill = false;
bool Robot::ROBOT_IS_ABOUT_TO_TIP = false;
int Robot::I[];	// our identity array

Robot::Robot() {
	instance = this;

	mp_position = new SendableChooser();
	mp_defense = new SendableChooser();
	mp_target = new SendableChooser();

	mp_autonomousCommand = NULL;

	mp_operatorInterface = new OI();
	ticks = 0;
}

Robot::~Robot() {
}

void Robot::RobotInit() {
	CommandBase::init();

	mp_operatorInterface->registerCommands();

	CommandBase::visionSubsystem->camerasOn();

	// initialize our identity array
	for (int i=0; i!=sizeof(I)/sizeof(int); i++) I[i] = i;

	mp_position->AddDefault("1", &I[POS_ONE]);
	mp_position->AddObject("2", &I[POS_TWO]);
	mp_position->AddObject("3", &I[POS_THREE]);
	mp_position->AddObject("4", &I[POS_FOUR]);
	mp_position->AddObject("5", &I[POS_FIVE]);

	mp_defense->AddDefault("Low Bar", &I[DEF_LOW_BAR]);
	mp_defense->AddObject("Portcullis", &I[DEF_PORTCULLIS]);
	mp_defense->AddObject("Cheval de Frise", &I[DEF_CHEVAL]);
	mp_defense->AddObject("Moat", &I[DEF_MOAT]);
	mp_defense->AddObject("Ramparts", &I[DEF_RAMPARTS]);
	mp_defense->AddObject("Drawbridge", &I[DEF_DRAWBRIDGE]);
	mp_defense->AddObject("Sally Port", &I[DEF_SALLY_PORT]);
	mp_defense->AddObject("Rock Wall", &I[DEF_ROCK_WALL]);
	mp_defense->AddObject("Rough Terrain", &I[DEF_ROUGH_TERRAIN]);

	mp_target->AddDefault("Left", &I[TARGET_LEFT]);
	mp_target->AddObject("Center", &I[TARGET_CENTER]);
	mp_target->AddObject("Right", &I[TARGET_RIGHT]);

	SmartDashboard::PutData("Auto Position", mp_position);
	SmartDashboard::PutData("Auto Defense", mp_defense);
	SmartDashboard::PutData("Auto Target", mp_target);

	if (!isRoadkill)
		CommandBase::compressorSubsystem->setCompressor(true);
	else
		CommandBase::compressorSubsystem->setCompressor(false);

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

	CommandBase::flapSubsystem->setFlapsFraction(0.0); // default to flaps down

	printf("Done\n");
}

void Robot::AlwaysPeriodic() {
	ticks++;

	// Check to see if the robot is about to tip
	ROBOT_IS_ABOUT_TO_TIP = CommandBase::navXSubsystem->isRobotAboutToTip();

	// other stuff
	CommandBase::visionSubsystem->updateVision(Robot::ticks);

	CommandBase::visionSubsystem->sendValuesToSmartDashboard();
	CommandBase::driveSubsystem->sendValuesToSmartDashboard();
	CommandBase::navXSubsystem->sendValuesToSmartDashboard();
	CommandBase::shooterSubsystem->sendValuesToSmartDashboard();
	CommandBase::flapSubsystem->sendValuesToSmartDashboard();
	//CommandBase::pdpSubsystem->sendValuesToSmartDashboard();
	CommandBase::compressorSubsystem->sendValuesToSmartDashboard();
	CommandBase::wedgeSubsystem->sendValuesToSmartDashboard();
	CommandBase::intakeSubsystem->sendValuesToSmartDashboard();

//	if (mp_operatorInterface->joystickButtonPressed(
//			mp_operatorInterface->pXboxController, 3)) {
//		printf("Joystick 0:\n");
//		printf("\tName: %s\n",
//				DriverStation::GetInstance().GetJoystickName(0).c_str());
//		printf("\tType: %d\n", DriverStation::GetInstance().GetJoystickType(0));
//		printf("\tIsXbox: %d\n",
//				DriverStation::GetInstance().GetJoystickIsXbox(0));
//		printf("Joystick 1:\n");
//		printf("\tName: %s\n",
//				DriverStation::GetInstance().GetJoystickName(1).c_str());
//		printf("\tType: %d\n", DriverStation::GetInstance().GetJoystickType(1));
//		printf("\tIsXbox: %d\n",
//				DriverStation::GetInstance().GetJoystickIsXbox(1));
//	}
}

void Robot::DisabledInit() {
	printf("Robot: DisabledInit\n");
}

void Robot::DisabledPeriodic() {
	AlwaysPeriodic();
}

void Robot::AutonomousInit() {
	printf("Robot: AutononmousInit\n");

	int pos = *((int*) mp_position->GetSelected());
	int def = *((int*) mp_defense->GetSelected());
	int target = *((int*) mp_target->GetSelected());
	printf("Autonomous: Position %d | Defense %d | Target %d\n", pos, def,
			target);

	if (mp_autonomousCommand != NULL) {
		delete mp_autonomousCommand;
		mp_autonomousCommand = NULL;
	}
	mp_autonomousCommand = new AutonomousDriveSequence(pos, def, target);
	mp_autonomousCommand->Start();

	CommandBase::navXSubsystem->getNavX()->ZeroYaw(); // assume robot starts facing directly forward
	CommandBase::navXSubsystem->getNavX()->ResetDisplacement();

	CommandBase::driveJoystickCommand->Cancel();
	CommandBase::intakeRollerCommand->Cancel();
	CommandBase::flapCommand->Cancel();
}

void Robot::AutonomousPeriodic() {
	AlwaysPeriodic();
	Scheduler::GetInstance()->Run();
}

void Robot::TeleopInit() {
	printf("Robot: TeleopInit\n");
	if (mp_autonomousCommand != NULL) {
		mp_autonomousCommand->Cancel();
	}
	CommandBase::driveJoystickCommand->Start();
	CommandBase::intakeRollerCommand->Start();
	CommandBase::flapCommand->Start();
}

void Robot::TeleopPeriodic() {
	AlwaysPeriodic();
	Scheduler::GetInstance()->Run();
}

void Robot::TestPeriodic() {
	LiveWindow::GetInstance()->Run();
}

START_ROBOT_CLASS(Robot)

