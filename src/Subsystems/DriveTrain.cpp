#include <Stronghold2016Robot.h>

DriveTrain::DriveTrain() :
		Subsystem("DriveTrain") {
	printf("DriveTrain: initialize\n");
	printf("\tMaster talons\n");
	mp_left1.reset(new CANTalon(RobotMap::CT_DRIVE_LEFT1));
	mp_right1.reset(new CANTalon(RobotMap::CT_DRIVE_RIGHT1));
	printf("\t2nd talons\n");
	mp_left2.reset(new CANTalon(RobotMap::CT_DRIVE_LEFT2));
	mp_right2.reset(new CANTalon(RobotMap::CT_DRIVE_RIGHT2));
	printf("\t3rd talons\n");
	mp_left3.reset(new CANTalon(RobotMap::CT_DRIVE_LEFT3));
	mp_right3.reset(new CANTalon(RobotMap::CT_DRIVE_RIGHT3));

	CANTalon* talonPtrs[7];
	talonPtrs[RobotMap::CT_DRIVE_LEFT1] = mp_left1.get();
	talonPtrs[RobotMap::CT_DRIVE_LEFT2] = mp_left2.get();
	talonPtrs[RobotMap::CT_DRIVE_LEFT3] = mp_left3.get();
	talonPtrs[RobotMap::CT_DRIVE_RIGHT1] = mp_right1.get();
	talonPtrs[RobotMap::CT_DRIVE_RIGHT2] = mp_right2.get();
	talonPtrs[RobotMap::CT_DRIVE_RIGHT3] = mp_right3.get();

	bool talonsPresent[7];
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

	mp_robotDrive.reset(new RobotDrive(masterLeft, masterRight));
	mp_robotDrive->SetSafetyEnabled(false);
	mp_robotDrive->SetExpiration(0.1);
	mp_robotDrive->SetSensitivity(0.5);
	mp_robotDrive->SetMaxOutput(1);
}

DriveTrain::~DriveTrain() {
}

void DriveTrain::InitDefaultCommand() {
}

void DriveTrain::ZeroMotors() {
	Drive(0, 0);
}

void DriveTrain::Drive(double y, double x) {
	if (mp_robotDrive.get() == NULL)
		return;
	mp_robotDrive->ArcadeDrive(y, x);
}

void DriveTrain::DriveSplitJoystick(Joystick* p_accelerationJoystick,
		Joystick* p_turnJoystick) {
	Drive(p_accelerationJoystick->GetY(), p_turnJoystick->GetX());
}

void DriveTrain::TalonMasterInit(CANTalon& r_talon) {
	r_talon.SetControlMode(CANTalon::kPercentVbus);
}

void DriveTrain::TalonSlaveInit(CANTalon& r_slaveTalon, int masterId) {
	r_slaveTalon.SetControlMode(CANTalon::kFollower);
	r_slaveTalon.Set(masterId);
	r_slaveTalon.EnableControl();
}

bool DriveTrain::IsTalonPresent(CANTalon& r_talon) {
	r_talon.GetOutputVoltage();
	return r_talon.GetError().GetCode() == 0;
}
