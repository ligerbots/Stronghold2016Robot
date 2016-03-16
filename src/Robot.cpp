#include <Stronghold2016Robot.h>
#include <time.h>

// Robot wide globals whose definitions live in the Robot class
Robot* Robot::instance = NULL;
int Robot::ticks;
bool Robot::isRoadkill = false;
bool Robot::ROBOT_IS_ABOUT_TO_TIP = false;
timespec Robot::resolution;

Robot::Robot() {
	instance = this;

	mp_autonomousCommand = NULL;
	mp_operatorInterface = new OI();
	ticks = 0;
	m_startTicks = 0;
	m_startTime = 0.0;
	clock_getres(CLOCK_REALTIME, &resolution);
	printf("Robot::Robot clock resolution %ld nanoseconds", resolution.tv_nsec);
}

Robot::~Robot() {
}

void Robot::RobotInit() {
	// We zero our measuring counters here. We're only interested in how long
	// an autonomous or teleop session takes, not how long the robot sits idle.
	m_startTicks = 0;
	m_startTime = 0.0;

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
	double startTime = GetRTC();
	printf("Robot: Disabled at %f seconds, %d ticks", startTime, ticks);
	// If we'd previously run an autonomous, record how long it lasted
	if (m_startTicks > 0) {
		double elapsedTime = startTime - m_startTime;
		int elapsedTicks = ticks - m_startTicks;
		printf(", prior state ran for %f seconds, %d ticks, %5.2f tps.",
				elapsedTime, elapsedTicks, elapsedTicks/elapsedTime);
	}
	printf("\n");
	m_startTicks = ticks;
	m_startTime = startTime;

	if (mp_autonomousCommand != NULL) {
		mp_autonomousCommand->Cancel();
	}

	// cbf -- commented this all out so we don't keep on getting SPI CRC errors
	// spamming our logs
	printf("Writing i2c\n");
	I2C i2c(I2C::kOnboard, 10);

	int numLeds = 23;
//	int numStrips = 2;


	char startByte = 0b11000000;
	char r = 0, g = 0, b = 0;

	uint8_t bytes[numLeds * 3 + 1];
	bytes[0] = startByte;
	for (int j = 1; j < numLeds * 3 + 1; j += 3) {
		bytes[j] = r;
		bytes[j + 1] = g;
		bytes[j + 2] = b;
	}
	i2c.WriteBulk(bytes, numLeds * 3 + 1);
}

void Robot::DisabledPeriodic() {
	AlwaysPeriodic();
}

void Robot::AutonomousInit() {
	m_startTicks = ticks;
	m_startTime = GetRTC();
	printf("Robot: AutononmousInit at %f seconds, %d ticks\n",
			m_startTime, ticks);

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
	double startTime = GetRTC();
	printf("Robot: TeleopInit at %f seconds, %d ticks", startTime, ticks);
	// If we'd previously run an autonomous, record how long it lasted
	if (m_startTicks > 0) {
		double elapsedTime = startTime - m_startTime;
		int elapsedTicks = ticks - m_startTicks;
		printf(", prior autonomous ran for %f seconds, %d ticks, %5.2f tps.",
				elapsedTime, elapsedTicks, elapsedTicks/elapsedTime);
	}
	printf("\n");
	m_startTicks = ticks;
	m_startTime = startTime;

	if (mp_autonomousCommand != NULL) {
		mp_autonomousCommand->Cancel();
	}
	CommandBase::driveJoystickCommand->Start();
	CommandBase::intakeRollerCommand->Start();
	CommandBase::flapCommand->Start();
	CommandBase::navXSubsystem->zeroYaw(Robot::instance->fieldInfo.GetInitialOrientation());
}

void Robot::TeleopPeriodic() {
	AlwaysPeriodic();
	Scheduler::GetInstance()->Run();
}

void Robot::TestPeriodic() {
	LiveWindow::GetInstance()->Run();
}

double Robot::GetRTC() {
	timespec timeSpec;
	clock_gettime(CLOCK_REALTIME, &timeSpec);
	return (double)timeSpec.tv_sec + (double)timeSpec.tv_nsec/1.0E9;
}

START_ROBOT_CLASS(Robot)

