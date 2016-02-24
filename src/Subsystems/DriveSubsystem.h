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

	std::unique_ptr<DoubleSolenoid> mp_shifterSolenoid;

	CANTalon *talonPtrs[7];
	bool talonsPresent[7];

	CANTalon *mp_leftEncoder;
	CANTalon *mp_rightEncoder;

protected:
	class DriveTurnPIDOutput: public PIDOutput {
	protected:
		DriveSubsystem& mr_parent;
	public:
		explicit DriveTurnPIDOutput(DriveSubsystem& r_parent) :
			mr_parent(r_parent) {
		}
		void PIDWrite(float pidOutput) {
			printf("TurnPIDOutput %f\n", pidOutput);
			if(pidOutput > 0.7) pidOutput = 0.7;
			if(pidOutput > 0 && pidOutput < 0.33) pidOutput = 0.33;
			mr_parent.drive(0, pidOutput); // turn
		}
	};

	/**
	 * Initialize a talon as a master
	 * @param r_talon The talon
	 */
	void TalonMasterInit(CANTalon& r_talon);
	/**
	 * Initialize a talon as a slave
	 * @param r_slaveTalon The talon
	 * @param masterId The CAN ID of the master talon to copy from
	 */
	void TalonSlaveInit(CANTalon& r_slaveTalon, int masterId);
	/**
	 * Checks if a talon is present (responds to CAN bus messages)
	 * @param r_talon The talon to check
	 * @return True if the talon is present, false otherwise
	 */
	bool IsTalonPresent(CANTalon& r_talon);bool IsEncoderPresent(
			CANTalon& r_talon);

public:
	DriveSubsystem();
	virtual ~DriveSubsystem();
	void InitDefaultCommand();
	/**
	 * Drive with the specified x and y inputs
	 * @param y Linear, [0, 1]
	 * @param x Rotation, [0, 1]
	 */
	void drive(double y, double x);
	void driveDirect(double left, double right);
	/**
	 * Zeroes the motors. Use at the end of autonomous commands that use DriveSubsystem
	 */
	void zeroMotors();
	void shiftUp();
	void shiftDown();
	double getLeftEncoderPosition();
	double getRightEncoderPosition();

	/**
	 * PIDOutput instance to use for turning
	 */
	std::shared_ptr<DriveTurnPIDOutput> turnPIDOutput;

	void sendValuesToSmartDashboard();
};
