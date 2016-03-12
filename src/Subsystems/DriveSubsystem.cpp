#include <Stronghold2016Robot.h>

DriveSubsystem::DriveSubsystem() :
		Subsystem("DriveSubsystem"){
	printf("DriveSubsystem: initialize\n");
	turnPIDOutput.reset(new DriveTurnPIDOutput(*this));
	// create all the CANTalon objects we need
	// we'll check if we can use them later
	printf("\tMaster talons\n");
	mp_left1.reset(new CANTalon(RobotMap::CT_DRIVE_LEFT1));
	mp_right1.reset(new CANTalon(RobotMap::CT_DRIVE_RIGHT1));
	printf("\t2nd talons\n");
	mp_left2.reset(new CANTalon(RobotMap::CT_DRIVE_LEFT2));
	mp_right2.reset(new CANTalon(RobotMap::CT_DRIVE_RIGHT2));
	printf("\t3rd talons\n");
	mp_left3.reset(new CANTalon(RobotMap::CT_DRIVE_LEFT3));
	mp_right3.reset(new CANTalon(RobotMap::CT_DRIVE_RIGHT3));

	// these arrays depend on the CAN IDs being 1-6
	talonPtrs[RobotMap::CT_DRIVE_LEFT1] = mp_left1.get();
	talonPtrs[RobotMap::CT_DRIVE_LEFT2] = mp_left2.get();
	talonPtrs[RobotMap::CT_DRIVE_LEFT3] = mp_left3.get();
	talonPtrs[RobotMap::CT_DRIVE_RIGHT1] = mp_right1.get();
	talonPtrs[RobotMap::CT_DRIVE_RIGHT2] = mp_right2.get();
	talonPtrs[RobotMap::CT_DRIVE_RIGHT3] = mp_right3.get();

	// check if all the talons are present by trying to get output voltage
	talonsPresent[RobotMap::CT_DRIVE_LEFT1] = IsTalonPresent(*mp_left1);
	talonsPresent[RobotMap::CT_DRIVE_LEFT2] = IsTalonPresent(*mp_left2);
	talonsPresent[RobotMap::CT_DRIVE_LEFT3] = IsTalonPresent(*mp_left3);
	talonsPresent[RobotMap::CT_DRIVE_RIGHT1] = IsTalonPresent(*mp_right1);
	talonsPresent[RobotMap::CT_DRIVE_RIGHT2] = IsTalonPresent(*mp_right2);
	talonsPresent[RobotMap::CT_DRIVE_RIGHT3] = IsTalonPresent(*mp_right3);

	printf("\tTalons: %d %d %d | %d %d %d\n",
			talonsPresent[RobotMap::CT_DRIVE_LEFT1],
			talonsPresent[RobotMap::CT_DRIVE_LEFT2],
			talonsPresent[RobotMap::CT_DRIVE_LEFT3],
			talonsPresent[RobotMap::CT_DRIVE_RIGHT1],
			talonsPresent[RobotMap::CT_DRIVE_RIGHT2],
			talonsPresent[RobotMap::CT_DRIVE_RIGHT3]);

	// CAN IDs
	int masterLeft;
	int masterRight;
	int slave1Left;
	int slave1Right;
	int slave2Left;
	int slave2Right;

	// find which talons are present and can be used as master talons
	if (talonsPresent[RobotMap::CT_DRIVE_LEFT1]) {
		masterLeft = RobotMap::CT_DRIVE_LEFT1;
		slave1Left = RobotMap::CT_DRIVE_LEFT2;
		slave2Left = RobotMap::CT_DRIVE_LEFT3;
	} else if (talonsPresent[RobotMap::CT_DRIVE_LEFT2]) {
		masterLeft = RobotMap::CT_DRIVE_LEFT2;
		slave1Left = RobotMap::CT_DRIVE_LEFT1;
		slave2Left = RobotMap::CT_DRIVE_LEFT3;
	} else if (talonsPresent[RobotMap::CT_DRIVE_LEFT3]) {
		masterLeft = RobotMap::CT_DRIVE_LEFT3;
		slave1Left = RobotMap::CT_DRIVE_LEFT2;
		slave2Left = RobotMap::CT_DRIVE_LEFT1;
	} else {
		printf("No left talons present\n");
		return; // don't create a RobotDrive. Drive() checks if it's null
	}

	if (talonsPresent[RobotMap::CT_DRIVE_RIGHT1]) {
		masterRight = RobotMap::CT_DRIVE_RIGHT1;
		slave1Right = RobotMap::CT_DRIVE_RIGHT2;
		slave2Right = RobotMap::CT_DRIVE_RIGHT3;
	} else if (talonsPresent[RobotMap::CT_DRIVE_RIGHT2]) {
		masterRight = RobotMap::CT_DRIVE_RIGHT2;
		slave1Right = RobotMap::CT_DRIVE_RIGHT1;
		slave2Right = RobotMap::CT_DRIVE_RIGHT3;
	} else if (talonsPresent[RobotMap::CT_DRIVE_RIGHT3]) {
		masterRight = RobotMap::CT_DRIVE_RIGHT3;
		slave1Right = RobotMap::CT_DRIVE_RIGHT2;
		slave2Right = RobotMap::CT_DRIVE_RIGHT1;
	} else {
		printf("No right talons present\n");
		return;
	}

	TalonMasterInit(*(talonPtrs[masterLeft]));
	TalonMasterInit(*(talonPtrs[masterRight]));

	// set the other talons to slave talons
	if (talonsPresent[slave1Left]) {
		TalonSlaveInit(*(talonPtrs[slave1Left]), masterLeft);
	}
	if (talonsPresent[slave2Left]) {
		TalonSlaveInit(*(talonPtrs[slave2Left]), masterLeft);
	}
	if (talonsPresent[slave1Right]) {
		TalonSlaveInit(*(talonPtrs[slave1Right]), masterRight);
	}
	if (talonsPresent[slave2Right]) {
		TalonSlaveInit(*(talonPtrs[slave2Right]), masterRight);
	}

	/*
	// Figure out where the encoders are
	// Left
	if (IsEncoderPresent(*mp_left1)) {
		mp_leftEncoder = mp_left1.get();
		printf("Left is 1\n");
	} else if (IsEncoderPresent(*mp_left2)) {
		mp_leftEncoder = mp_left2.get();
		printf("Left is 2\n");
	} else if (IsEncoderPresent(*mp_left3)) {
		mp_leftEncoder = mp_left3.get();
		printf("Left is 3\n");
	} else {
		printf("Warning: No left drive encoder found\n");
		mp_leftEncoder = NULL;
	}
	// Right
	if (IsEncoderPresent(*mp_right1)) {
		mp_rightEncoder = mp_right1.get();
		printf("Right is 1\n");
	} else if (IsEncoderPresent(*mp_right2)) {
		mp_rightEncoder = mp_right2.get();
		printf("Right is 2\n");
	} else if (IsEncoderPresent(*mp_right3)) {
		mp_rightEncoder = mp_right3.get();
		printf("Right is 3\n");
	} else {
		printf("Warning: No right drive encoder found\n");
		mp_rightEncoder = NULL;
	}*/

	// detect roadkill if both 3rd talons are missing
	if(!talonsPresent[RobotMap::CT_DRIVE_LEFT3]
			&& !talonsPresent[RobotMap::CT_DRIVE_RIGHT3]){
		Robot::isRoadkill = true;
		printf("Roadkill detected\n");
	}

	mp_leftEncoder = talonPtrs[RobotMap::CT_DRIVE_LEFT1];
	mp_rightEncoder = talonPtrs[RobotMap::CT_DRIVE_RIGHT1];

	// the object that actually handles setting talons from Arcade Drive input
	mp_robotDrive.reset(
			new RobotDrive(talonPtrs[masterLeft], talonPtrs[masterRight]));
	mp_robotDrive->SetSafetyEnabled(false);
	mp_robotDrive->SetExpiration(0.2); // 2nd robot is jumping if this is .1
	mp_robotDrive->SetSensitivity(0.5);
	mp_robotDrive->SetMaxOutput(1);

	// initializes the shifter solenoid - forward for high gear, reverse for low
	mp_shifterSolenoid.reset(
			new DoubleSolenoid(RobotMap::PCM_CAN,
					RobotMap::PCM_SHIFTER_HIGH_GEAR,
					RobotMap::PCM_SHIFTER_LOW_GEAR));

	printf("Drive init done\n");
}

DriveSubsystem::~DriveSubsystem() {
}

void DriveSubsystem::InitDefaultCommand() {
}

void DriveSubsystem::zeroMotors() {
	drive(0, 0);
}

void DriveSubsystem::shiftUp() {
	mp_shifterSolenoid->Set(DoubleSolenoid::kForward);
}

void DriveSubsystem::shiftDown() {
	printf("Drive: shifting down\n");
	mp_shifterSolenoid->Set(DoubleSolenoid::kReverse);
}

bool DriveSubsystem::isShiftedUp(){
	printf("Drive: shifting down\n");
	return mp_shifterSolenoid->Get() == DoubleSolenoid::kForward;
}

void DriveSubsystem::drive(double y, double x) {
	if (mp_robotDrive.get() == NULL)
		return;
	// Before we do anything, check to see if we're about to tip over
	if (Robot::ROBOT_IS_ABOUT_TO_TIP && y < 0.0) {
		// Don't let the robot go forward anymore
		y = 0.0;
	}
	SmartDashboard::PutNumber("x", x);
	SmartDashboard::PutNumber("y", y);
	mp_robotDrive->ArcadeDrive(y, x);
}

void DriveSubsystem::driveDirect(double left, double right){
	// Before we do anything, check to see if we're about to tip over
	if (Robot::ROBOT_IS_ABOUT_TO_TIP) {
		if (left > 0.0) left = 0.0;
		if (right > 0.0) right = 0.0;
	}
	mp_robotDrive->SetLeftRightMotorOutputs(left, right);
}

double DriveSubsystem::getLeftEncoderPosition() {
	if (mp_leftEncoder != NULL) {
		double pos = mp_leftEncoder->GetPosition();
		if (mp_leftEncoder->GetError().GetCode() == 0)
			return pos;
	}
	return NAN;
}

double DriveSubsystem::getRightEncoderPosition() {
	if (mp_rightEncoder != NULL) {
		double pos = mp_rightEncoder->GetPosition();
		if (mp_rightEncoder->GetError().GetCode() == 0)
			return pos;
	}
	return NAN;
}

void DriveSubsystem::TalonMasterInit(CANTalon& r_talon) {
	r_talon.SetControlMode(CANTalon::kPercentVbus);
}

void DriveSubsystem::TalonSlaveInit(CANTalon& r_slaveTalon, int masterId) {
	r_slaveTalon.SetControlMode(CANTalon::kFollower);
	r_slaveTalon.Set(masterId);
	r_slaveTalon.EnableControl();
}

bool DriveSubsystem::IsTalonPresent(CANTalon& r_talon) {
	r_talon.GetOutputVoltage();
	return r_talon.GetError().GetCode() == 0;
}

bool DriveSubsystem::IsEncoderPresent(CANTalon& r_talon) {
	r_talon.ClearError();
	r_talon.GetPosition();
	return r_talon.GetError().GetCode() == 0;
}

void DriveSubsystem::sendValuesToSmartDashboard() {
	if (Robot::ticks % 2) {
		for (int i = 1; i < 7; i++) {
			std::string key = "Drive/Talon";
			key += std::to_string(i);
			key += "/";
			SmartDashboard::PutBoolean(key + "Status", talonsPresent[i]);
			// put zero watts if the talon isn't present
			SmartDashboard::PutNumber(key + "Watts",
					talonsPresent[i] ?
							talonPtrs[i]->GetOutputCurrent()
									* talonPtrs[i]->GetOutputVoltage() :
							0.0);
		}

		SmartDashboard::PutNumber("Drive/LeftPosition",
				getLeftEncoderPosition());
		SmartDashboard::PutNumber("Drive/RightPosition",
				getRightEncoderPosition());

		if (mp_shifterSolenoid->GetError().GetCode() != 0) {
			SmartDashboard::PutString("Drive/Shifter", "Not Present");
		} else {
			DoubleSolenoid::Value val = mp_shifterSolenoid->Get();
			if (val == DoubleSolenoid::kOff) {
				SmartDashboard::PutString("Drive/Shifter", "Off");
			} else if (val == DoubleSolenoid::kForward) {
				SmartDashboard::PutString("Drive/Shifter", "High Gear");
			} else if (val == DoubleSolenoid::kReverse) {
				SmartDashboard::PutString("Drive/Shifter", "Low Gear");
			}
		}
	}
}
