#include <Stronghold2016Robot.h>
#include <time.h>

// Robot wide globals whose definitions live in the Robot class
Robot* Robot::instance = NULL;
int Robot::ticks;
bool Robot::isRoadkill = false;
bool Robot::ROBOT_IS_ABOUT_TO_TIP = false;
timespec Robot::resolution;

Robot::Robot() :
		ledsCommunication(I2C::kOnboard, 42) {
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

	// set the robot initial position (perhaps)
	CommandBase::navXSubsystem->zeroYaw(fieldInfo.GetInitialOrientation());

	printf("Done\n");
}

void Robot::AlwaysPeriodic() {
	ticks++;

//	if(ticks % 50 == 0){
//		printf("Main thread running\n");
//	}

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

	if (mp_operatorInterface->get2ndControllerButton(28)) {
		printf("Canceling centering/auto\n");
		if(mp_autonomousCommand != NULL){
			mp_autonomousCommand->Cancel();
		}
		CommandBase::centerOnTargetCommand->Cancel();
		if(DriverStation::GetInstance().IsOperatorControl()){
			CommandBase::driveJoystickCommand->Start();
			CommandBase::flapCommand->Start();
			CommandBase::intakeRollerCommand->Start();
		}
	}

	if (mp_operatorInterface->get2ndControllerButton(21)) {
		CommandBase::visionSubsystem->setVisionEnabled(true);
	}

	if (mp_operatorInterface->get2ndControllerButton(24)) {
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

	printf("LEDs: Turning off\n");
	SetLeds(OFF);
}

void Robot::SetLeds(LedState state){
	uint8_t bytes[10];
	switch(state){
	case OFF:
		bytes[0] = 'O';
		break;
	case NORMAL:
		if(DriverStation::GetInstance().IsAutonomous()){
			bytes[0] = 'A';
		} else if(DriverStation::GetInstance().GetAlliance() == DriverStation::kRed){
			bytes[0] = 'R';
		} else {
			bytes[0] = 'B';
		}
		break;
	case SHOOT:
		bytes[0] = 'S';
		break;
	}
	// write the same byte a few more times for good measure
	// during the brief LED testing time we had at WPI, the I2C bus was
	// dropping 50% of a transmission
	for(int i = 1; i < 10; i++){
		bytes[i] = bytes[0];
	}
	ledsCommunication.WriteBulk(bytes, 1);
}

void Robot::DisabledPeriodic() {
	AlwaysPeriodic();
	// the commands don't seem to run in disabled
	if(mp_operatorInterface->pFarmController->GetRawButton(22)){
		CommandBase::visionSubsystem->setCameraFeed(0);
	}
	if(mp_operatorInterface->pFarmController->GetRawButton(23)){
		CommandBase::visionSubsystem->setCameraFeed(1);
	}

	if(ticks % 50 == 0){
		printf("Selected auto parameters:\n");
		printf("\tPosition: %d %s\n",
				fieldInfo.GetPosition(), FieldInfo::StartingPositionNames[fieldInfo.GetPosition()].c_str());
		printf("\tDefense: %d %s\n",
				fieldInfo.GetDefense(), FieldInfo::DefenseNames[fieldInfo.GetDefense()].c_str());
		printf("\tTarget: %d %s\n",
				fieldInfo.GetTarget(), FieldInfo::TargetNames[fieldInfo.GetTarget()].c_str());
	}
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

	CommandBase::driveSubsystem->SetInitialPosition(FieldInfo::startingLocations[pos].x,
													FieldInfo::startingLocations[pos].y);

	CommandBase::driveJoystickCommand->Cancel();
	CommandBase::intakeRollerCommand->Cancel();
	CommandBase::flapCommand->Cancel();
	SetLeds(NORMAL);
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

	// uncomment only for calibrating vision
//	CommandBase::navXSubsystem->zeroYaw(0);

	if (mp_autonomousCommand != NULL) {
		mp_autonomousCommand->Cancel();
	}
	CommandBase::driveJoystickCommand->Start();
	CommandBase::intakeRollerCommand->Start();
	CommandBase::flapCommand->Start();
	SetLeds(NORMAL);
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

