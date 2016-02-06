#pragma once

#include "Commands/Subsystem.h"
#include "WPILib.h"

class DriveSubsystem: public Subsystem {
private:
	std::unique_ptr<CANTalon> mp_left1;
	std::unique_ptr<CANTalon> mp_left2;
	std::unique_ptr<CANTalon> mp_left3;
	std::unique_ptr<CANTalon> mp_right1;
	std::unique_ptr<CANTalon> mp_right2;
	std::unique_ptr<CANTalon> mp_right3;

	std::unique_ptr<RobotDrive> mp_robotDrive;

protected:
	void TalonMasterInit(CANTalon& r_talon);
	void TalonSlaveInit(CANTalon& r_slaveTalon, int masterId);
	bool IsTalonPresent(CANTalon& r_talon);

public:
	DriveSubsystem();
	virtual ~DriveSubsystem();
	void InitDefaultCommand();
	void drive(double y, double x);
	void zeroMotors();
	void shiftUp();
	void shiftDown();
	double getLeftEncoderPosition();
	double getRightEncoderPosition();

};
