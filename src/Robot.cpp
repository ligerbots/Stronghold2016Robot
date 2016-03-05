#include <Stronghold2016Robot.h>

// Robot wide globals whose definitions live in the Robot class
Robot* Robot::instance = NULL;
int Robot::ticks;
bool Robot::isRoadkill = false;

int Robot::POS_ONE = 1;
int Robot::POS_TWO = 2;
int Robot::POS_THREE = 3;
int Robot::POS_FOUR = 4;
int Robot::POS_FIVE = 5;
int Robot::DEF_LOW_BAR = 6;
int Robot::DEF_PORTCULLIS = 7;
int Robot::DEF_CHEVAL = 8;
int Robot::DEF_MOAT = 9;
int Robot::DEF_RAMPARTS = 10;
int Robot::DEF_DRAWBRIDGE = 11;
int Robot::DEF_SALLY_PORT = 12;
int Robot::DEF_ROCK_WALL = 13;
int Robot::DEF_ROUGH_TERRAIN = 14;
int Robot::TARGET_LEFT = 15;
int Robot::TARGET_CENTER = 16;
int Robot::TARGET_RIGHT = 17;

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

	mp_position->AddDefault("1", &POS_ONE);
	mp_position->AddObject("2", &POS_TWO);
	mp_position->AddObject("3", &POS_THREE);
	mp_position->AddObject("4", &POS_FOUR);
	mp_position->AddObject("5", &POS_FIVE);

	mp_defense->AddDefault("Low Bar", &DEF_LOW_BAR);
	mp_defense->AddObject("Portcullis", &DEF_PORTCULLIS);
	mp_defense->AddObject("Cheval de Frise", &DEF_CHEVAL);
	mp_defense->AddObject("Moat", &DEF_MOAT);
	mp_defense->AddObject("Ramparts", &DEF_RAMPARTS);
	mp_defense->AddObject("Drawbridge", &DEF_DRAWBRIDGE);
	mp_defense->AddObject("Sally Port", &DEF_SALLY_PORT);
	mp_defense->AddObject("Rock Wall", &DEF_ROCK_WALL);
	mp_defense->AddObject("Rough Terrain", &DEF_ROUGH_TERRAIN);

	mp_target->AddDefault("Left", &TARGET_LEFT);
	mp_target->AddObject("Center", &TARGET_CENTER);
	mp_target->AddObject("Right", &TARGET_RIGHT);

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

