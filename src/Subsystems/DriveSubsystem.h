#pragma once

#include "Commands/Subsystem.h"
#include "WPILib.h"
#include "Robot.h"

class DriveSubsystem: public Subsystem {
protected:
	class DriveTurnPIDOutput: public PIDOutput {
	protected:
		DriveSubsystem& mr_parent;
	public:
		explicit DriveTurnPIDOutput(DriveSubsystem& r_parent) :
			mr_parent(r_parent) {
		}
		void PIDWrite(float pidOutput) {
//			if(fabs(pidOutput) > 0 && fabs(pidOutput) < 0.38){
//				if(pidOutput < 0)
//					pidOutput = -0.38;
//				else
//					pidOutput = 0.38;
//			}
			if(fabs(pidOutput) > .9) {
				pidOutput = pidOutput < 0 ? -.9 : .9;
			}

			pidOutput = -pidOutput;

			if (Robot::ticks%5==0) printf("TurnPIDOutput %f\n", pidOutput);
			mr_parent.drive(0, pidOutput); // turn
		}
	};

public:

	// structure used for tracking robot position
	struct Position {
		double X, Y;
		double Angle;
	};
	Position m_pos;
	double m_originalDistance;	// original encoder value in inches

	// m_prevDistance is adjusted by original distance, so it starts
	// at zero and records the distance the robot traveled since
	// the last tick
	double m_prevDistance;

	// empirically measured by driving 5 feet
	static constexpr double TICKS_PER_INCH = 640;

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
	bool isShiftedUp();
	double getLeftEncoderPosition();
	double getRightEncoderPosition();
	void updatePosition();
	void SetInitialPosition(double x, double y);
	Position GetPosition();

	/**
	 * PIDOutput instance to use for turning (not really)
	 */
	std::shared_ptr<DriveTurnPIDOutput> turnPIDOutput;

	void sendValuesToSmartDashboard();

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
	bool IsTalonPresent(CANTalon& r_talon);
	bool IsEncoderPresent(
			CANTalon& r_talon);

};
