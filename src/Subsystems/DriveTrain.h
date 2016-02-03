#pragma once

#include "Commands/Subsystem.h"
#include "WPILib.h"

class DriveTrain: public Subsystem {
private:
	std::unique_ptr<CANTalon> mp_left1;
	std::unique_ptr<CANTalon> mp_left2;
	std::unique_ptr<CANTalon> mp_left3;
	std::unique_ptr<CANTalon> mp_right1;
	std::unique_ptr<CANTalon> mp_right2;
	std::unique_ptr<CANTalon> mp_right3;

	RobotDrive m_robotDrive;

protected:
	void TalonMasterInit(CANTalon& r_talon);
	void TalonSlaveInit(CANTalon& r_slaveTalon, int masterId);
	bool IsTalonPresent(CANTalon& r_talon);

public:
	DriveTrain();
	void InitDefaultCommand();
	void Drive(double y, double x);
	void DriveSplitJoystick(Joystick* p_accelerationJoystick, Joystick* p_turnJoystick);
	void ZeroMotors();
};
