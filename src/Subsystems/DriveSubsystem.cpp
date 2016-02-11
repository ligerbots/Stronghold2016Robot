#include <Stronghold2016Robot.h>

DriveSubsystem::DriveSubsystem() :
		Subsystem("DriveSubsystem") {
	printf("DriveSubsystem: initialize\n");
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

	// the object that actually handles setting talons from Arcade Drive input
	mp_robotDrive.reset(new RobotDrive(masterLeft, masterRight));
	mp_robotDrive->SetSafetyEnabled(false);
	mp_robotDrive->SetExpiration(0.1);
	mp_robotDrive->SetSensitivity(0.5);
	mp_robotDrive->SetMaxOutput(1);

	// TODO: initialize mp_shifterSolenoid here
}

DriveSubsystem::~DriveSubsystem() {
}

void DriveSubsystem::InitDefaultCommand() {
}

void DriveSubsystem::zeroMotors() {
	drive(0, 0);
}

void DriveSubsystem::drive(double y, double x) {
	if (mp_robotDrive.get() == NULL)
		return;
	mp_robotDrive->ArcadeDrive(y, x);
}

double DriveSubsystem::getLeftEncoderPosition() {
	if (IsEncoderPresent(*mp_left1)) {
		return mp_left1->GetPosition();
	} else if (IsEncoderPresent(*mp_left2)) {
		return mp_left2->GetPosition();
	} else if (IsEncoderPresent(*mp_left3)) {
		return mp_left3->GetPosition();
	} else {
		return NAN;
	}
}

double DriveSubsystem::getRightEncoderPosition() {
	if (IsEncoderPresent(*mp_right1)) {
		return mp_right1->GetPosition();
	} else if (IsEncoderPresent(*mp_right2)) {
		return mp_right2->GetPosition();
	} else if (IsEncoderPresent(*mp_right3)) {
		return mp_right3->GetPosition();
	} else {
		return NAN;
	}
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
	for (int i = 1; i < 7; i++) {
		std::string key = "Drive/Talon_";
		key += std::to_string(i);
		key += "/";
		SmartDashboard::PutBoolean(key + "/Status", talonsPresent[i]);
		SmartDashboard::PutNumber(key + "/Watts",
				talonPtrs[i]->GetOutputCurrent()
						* talonPtrs[i]->GetOutputVoltage());
	}
}
