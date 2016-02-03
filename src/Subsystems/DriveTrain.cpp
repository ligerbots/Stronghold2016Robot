#include <Stronghold2016Robot.h>

DriveTrain::DriveTrain() :
		Subsystem("DriveTrain") {
	mp_left1 = std::make_unique<CANTalon>(RobotMap::CT_DRIVE_LEFT1);
	mp_left2 = std::make_unique<CANTalon>(RobotMap::CT_DRIVE_LEFT2);
	mp_left3 = std::make_unique<CANTalon>(RobotMap::CT_DRIVE_LEFT3);
	mp_right1 = std::make_unique<CANTalon>(RobotMap::CT_DRIVE_RIGHT1);
	mp_right2 = std::make_unique<CANTalon>(RobotMap::CT_DRIVE_RIGHT2);
	mp_right3 = std::make_unique<CANTalon>(RobotMap::CT_DRIVE_RIGHT3);

	TalonMasterInit(*mp_left1);
	TalonMasterInit(*mp_right1);

	TalonSlaveInit(*mp_left2, RobotMap::CT_DRIVE_LEFT1);
	TalonSlaveInit(*mp_right2, RobotMap::CT_DRIVE_RIGHT1);

	if (IsTalonPresent(*mp_left3) && IsTalonPresent(*mp_right3)) {
		TalonSlaveInit(*mp_left3, RobotMap::CT_DRIVE_LEFT1);
		TalonSlaveInit(*mp_right3, RobotMap::CT_DRIVE_RIGHT1);
	}

	m_robotDrive(mp_left1, mp_left2);
	m_robotDrive.SetSafetyEnabled(false);
	m_robotDrive.SetExpiration(0.1);
	m_robotDrive.SetSensitivity(0.5);
	m_robotDrive.SetMaxOutput(1);
}

void DriveTrain::InitDefaultCommand() {
}

void DriveTrain::ZeroMotors(){
	Drive(0, 0);
}

void DriveTrain::Drive(double y, double x) {
	m_robotDrive.ArcadeDrive(y, x);
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
