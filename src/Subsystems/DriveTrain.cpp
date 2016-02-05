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

	printf("\tTalons: %d %d %d | %d %d %d\n",
			IsTalonPresent(*mp_left1),
			IsTalonPresent(*mp_left2),
			IsTalonPresent(*mp_left3),
			IsTalonPresent(*mp_right1),
			IsTalonPresent(*mp_right2),
			IsTalonPresent(*mp_right3));

	TalonMasterInit(*mp_left1);
	TalonMasterInit(*mp_right1);

	TalonSlaveInit(*mp_left2, RobotMap::CT_DRIVE_LEFT1);
	TalonSlaveInit(*mp_right2, RobotMap::CT_DRIVE_RIGHT1);

	if (IsTalonPresent(*mp_left3) && IsTalonPresent(*mp_right3)) {
		TalonSlaveInit(*mp_left3, RobotMap::CT_DRIVE_LEFT1);
		TalonSlaveInit(*mp_right3, RobotMap::CT_DRIVE_RIGHT1);
	} else {
		printf("\tRoadkill detected\n");
	}

	mp_robotDrive.reset(new RobotDrive(mp_left1.get(), mp_right1.get()));
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
