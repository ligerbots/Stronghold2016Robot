#pragma once

#include "Commands/Subsystem.h"
#include "WPILib.h"

class IntakeSubsystem: public Subsystem
{
private:
	std::unique_ptr<DoubleSolenoid> mp_intakeArmSolenoid;
	std::unique_ptr<CANTalon> mp_rollers;
	std::unique_ptr<DigitalInput> mp_intakeUpSwitch;
	std::unique_ptr<DigitalInput> mp_ballInShooterSwitch;
//	std::unique_ptr<Counter> mp_ballInShooterCounter;
	std::unique_ptr<DigitalInput> mp_ballInDefensePositionSwitch;
	std::unique_ptr<AnalogInput> mp_ballPositionSensor;
//	std::unique_ptr<Counter> mp_ballInDefensePositionCounter;

	bool m_rollerTalonPresent;

	double m_robotRTCWhenSetUp;
	bool m_intakeSwitchHitOnce;
	double m_robotRTCWhenIntakeSwitchHitOnce;

	static constexpr double ROLLER_SPEED = 1;
public:
	IntakeSubsystem();
	void InitDefaultCommand();
	void rollIn();
	void rollOut();
	void rollStop();
	void setRollSpeed(double speed);
	double getRollerRevolutions();
	bool isIntakeArmUp();
	DoubleSolenoid::Value getIntakeArmValue();
	bool isBallInShooterPosition();
	bool isShooterSwitchPressed();
	bool isBallInDefensesCrossingPosition();
	bool isIntakeReadyToFire();
	void setIntakeArmUp();
	void setIntakeArmDown();

	void sendValuesToSmartDashboard();
};
