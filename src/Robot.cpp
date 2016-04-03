#include <Stronghold2016Robot.h>
#include <time.h>

// Robot wide globals whose definitions live in the Robot class
Robot* Robot::instance = NULL;
int Robot::ticks;
bool Robot::is_roadkill = false;
double Robot::end_of_centering_yaw = 0;
bool Robot::robot_is_about_to_tip = false;
timespec Robot::time_resolution;

Robot::Robot() :
		m_ledTeensyCommunication(I2C::kOnboard, 42) {
	instance = this;

	mp_autonomousCommand = NULL;
	mp_operatorInterface = new OI();
	ticks = 0;
	m_startTicks = 0;
	m_startTime = 0.0;
	clock_getres(CLOCK_REALTIME, &time_resolution);
	printf("Robot::Robot clock resolution %ld nanoseconds", time_resolution.tv_nsec);
}

Robot::~Robot() {
}

void Robot::RobotInit() {
	// We zero our measuring counters here. We're only interested in how long
	// an autonomous or teleop session takes, not how long the robot sits idle.
	m_startTicks = 0;
	m_startTime = 0.0;

	CommandBase::init();

	// initialize all commands and turn cameras on
	// if we try to do any of this in the constructor, bad things happen
	mp_operatorInterface->registerCommands();
	CommandBase::visionSubsystem->camerasOn();

	// on roadkill, don't be annoying
	// roadkill detected by DriveSubsystem
	if (!is_roadkill) {
		CommandBase::compressorSubsystem->setCompressor(true);
	} else {
		CommandBase::compressorSubsystem->setCompressor(false);
	}

	CommandBase::flapSubsystem->setFlapsFraction(1.0); // default to flaps down

	// publish the auto selectors to SmartDashboard
	m_fieldInfo.initSelectors();

	// set the robot initial position (perhaps)
	CommandBase::navXSubsystem->zeroYaw(m_fieldInfo.GetInitialOrientation());

	printf("RobotInit() done\n");
}

void Robot::AlwaysPeriodic() {
	ticks++;

//	if(ticks % 50 == 0){
//		printf("Main thread running\n");
//	}

	// Check to see if the robot is about to tip
	// update the giant red-green indicator on the dashboard
	robot_is_about_to_tip = CommandBase::navXSubsystem->isRobotAboutToTip(RobotMap::MAX_PITCH_ANGLE);
	SmartDashboard::PutBoolean("SafeToDrive", !robot_is_about_to_tip);

	CommandBase::visionSubsystem->updateVision(Robot::ticks);

	// run through all subsystems and have them publish SmartDashboard values
	CommandBase::visionSubsystem->sendValuesToSmartDashboard();
	CommandBase::driveSubsystem->sendValuesToSmartDashboard();
	CommandBase::navXSubsystem->sendValuesToSmartDashboard();
	CommandBase::shooterSubsystem->sendValuesToSmartDashboard();
	CommandBase::flapSubsystem->sendValuesToSmartDashboard();
	// polling PDP channels tends to crash the program for some reason
	// perhaps because WPILib is always spamming the can bus to get the battery voltage
	//CommandBase::pdpSubsystem->sendValuesToSmartDashboard();
	CommandBase::compressorSubsystem->sendValuesToSmartDashboard();
	CommandBase::wedgeSubsystem->sendValuesToSmartDashboard();
	CommandBase::intakeSubsystem->sendValuesToSmartDashboard();

	// make red button force kill the auto command and centering, if they are running
	if (mp_operatorInterface->getSecondControllerRawButton(28)) {
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

	if (mp_operatorInterface->getSecondControllerRawButton(21)) {
		CommandBase::visionSubsystem->setVisionEnabled(true);
	}

	if (mp_operatorInterface->getSecondControllerRawButton(24)) {
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
	// the teensy program accepts single letters as bytes over I2C to set the animation state
	// for some reason about 50% of messages get lost over I2C so here we spam multiple copies of the letter
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
	printf("LEDs: Mode = %c\n", (char) (bytes[0]));
	// duplicate the byte
	for(int i = 1; i < 10; i++){
		bytes[i] = bytes[0];
	}
	m_ledTeensyCommunication.WriteBulk(bytes, 10);
}

void Robot::DisabledPeriodic() {
	AlwaysPeriodic();
	// the camera commands don't seem to run in disabled, even with SetRunInDisabled(true)
	if(mp_operatorInterface->mp_FarmController->GetRawButton(22)){
		CommandBase::visionSubsystem->setCameraFeed(0);
	}
	if(mp_operatorInterface->mp_FarmController->GetRawButton(23)){
		CommandBase::visionSubsystem->setCameraFeed(1);
	}

	if(ticks % 50 == 0){
		printf("Selected auto parameters:\n");
		printf("\tPosition: %d %s\n",
				m_fieldInfo.GetPosition(), FieldInfo::StartingPositionNames[m_fieldInfo.GetPosition()].c_str());
		printf("\tDefense: %d %s\n",
				m_fieldInfo.GetDefense(), FieldInfo::TargetNames[m_fieldInfo.GetDefense()].c_str());
		printf("\tTarget: %d %s\n",
				m_fieldInfo.GetTarget(), FieldInfo::DefenseNames[m_fieldInfo.GetTarget()].c_str());
	}
}

void Robot::AutonomousInit() {
	m_startTicks = ticks;
	m_startTime = GetRTC();
	printf("Robot: AutononmousInit at %f seconds, %d ticks\n",
			m_startTime, ticks);

	int pos = m_fieldInfo.GetPosition();
	int def = m_fieldInfo.GetDefense();
	int target = m_fieldInfo.GetTarget();
	bool slow = m_fieldInfo.IsCrossingSlowly();
	printf("Autonomous: Position %d | Defense %d | Target %d, Speed %s\n",
			pos, def, target, slow ? "SLOW" : "NORMAL");

	// make sure we don't inadvertently leave the LED ring off
	CommandBase::visionSubsystem->setLedRingState(true);

	// if we've already run auto, delete the existing auto command
	if (mp_autonomousCommand != NULL) {
		delete mp_autonomousCommand;
		mp_autonomousCommand = NULL;
	}
	// create a new dynamic auto command with the selected parameters from the SmartDashboard
	mp_autonomousCommand = new AutonomousDriveAndShoot(pos, def, target);
	mp_autonomousCommand->Start();

	// reset our coordinate system to where the robot is currently located / oriented
	// assume robot was set up correctly (there's nothing else we can do)
	// Robot will start out wedges first, or intake first, depending on the defense
	CommandBase::navXSubsystem->zeroYaw(m_fieldInfo.GetInitialOrientation());
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
	// begin teleop "polling" commands
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

