#include <Stronghold2016Robot.h>

// Robot wide globals whose definitions live in the Robot class
Robot* Robot::instance = NULL;
int Robot::ticks;
bool Robot::isRoadkill = false;
bool Robot::ROBOT_IS_ABOUT_TO_TIP = false;

Robot::Robot() {
	instance = this;

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

	if (!isRoadkill)
		CommandBase::compressorSubsystem->setCompressor(true);
	else
		CommandBase::compressorSubsystem->setCompressor(false);

	CommandBase::flapSubsystem->setFlapsFraction(1.0); // default to flaps down

	fieldInfo.initSelectors();

	printf("Done\n");
}

void Robot::AlwaysPeriodic() {
	ticks++;

	// Check to see if the robot is about to tip
	ROBOT_IS_ABOUT_TO_TIP = CommandBase::navXSubsystem->isRobotAboutToTip(RobotMap::MAX_PITCH_ANGLE);
	SmartDashboard::PutBoolean("SafeToDrive", !ROBOT_IS_ABOUT_TO_TIP);

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

	if(mp_operatorInterface->pFarmController->GetRawButton(28)){
		printf("Cancelling centering/auto\n");
		if(mp_autonomousCommand != NULL){
			mp_autonomousCommand->Cancel();
		}
		CommandBase::centerOnTargetCommand->Cancel();
	}

	if(mp_operatorInterface->pFarmController->GetRawButton(21)){
		CommandBase::visionSubsystem->setVisionEnabled(true);
	}

	if(mp_operatorInterface->pFarmController->GetRawButton(24)){
		CommandBase::visionSubsystem->setVisionEnabled(false);
	}

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

	if (mp_autonomousCommand != NULL) {
		mp_autonomousCommand->Cancel();
	}

	printf("Writing i2c\n");
	for (int i = 3; i < 4; i++) {
		I2C i2c(I2C::kOnboard, i);

		int numLeds = 23;
//		int numStrips = 2;

		printf("%d\n", i);

		char startByte = 0xC0;
		char r = 255, g = 0, b = 255;

		uint8_t bytes[numLeds * 3 + 1];
		bytes[0] = startByte;
		for (int j = 1; j < numLeds * 3 + 1; j += 3) {
			bytes[j] = r;
			bytes[j + 1] = g;
			bytes[j + 2] = b;
		}
		i2c.WriteBulk(bytes, numLeds * 3 + 1);
	}

}

void Robot::DisabledPeriodic() {
	AlwaysPeriodic();
}

void Robot::AutonomousInit() {
	printf("Robot: AutononmousInit\n");

	int pos = fieldInfo.GetPosition();
	int def = fieldInfo.GetDefense();
	int target = fieldInfo.GetTarget();
	bool slow = fieldInfo.CrossSlowly();
	printf("Autonomous: Position %d | Defense %d | Target %d, Speed %s\n",
			pos, def, target, slow ? "SLOW" : "NORMAL");

	// make sure we don't inadvertently leave the LED ring off
	CommandBase::visionSubsystem->setLedRingOn(true);

	if (mp_autonomousCommand != NULL) {
		delete mp_autonomousCommand;
		mp_autonomousCommand = NULL;
	}
	mp_autonomousCommand = new AutonomousDriveAndShoot(pos, def, target);
	mp_autonomousCommand->Start();

	// Robot will start out wedges first, or intake first, depending on the defense
	CommandBase::navXSubsystem->zeroYaw(fieldInfo.GetInitialOrientation());
	CommandBase::navXSubsystem->ResetDisplacement();

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
	CommandBase::navXSubsystem->zeroYaw(Robot::instance->fieldInfo.GetInitialOrientation());
}

void Robot::TeleopPeriodic() {
	timeval startTime;
	timeval endTime;
	gettimeofday(&startTime, 0);
	AlwaysPeriodic();
	Scheduler::GetInstance()->Run();
	gettimeofday(&endTime, 0);
	__suseconds_t diff = endTime.tv_usec - startTime.tv_usec;
	if (diff > -50000) {
		SmartDashboard::PutNumber("RobotLoop", 1/diff);
	}
}

void Robot::TestPeriodic() {
	LiveWindow::GetInstance()->Run();
}

START_ROBOT_CLASS(Robot)

