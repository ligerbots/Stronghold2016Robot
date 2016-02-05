#pragma once

#include <WPILib.h>
#include "OI.h"

class Robot: public IterativeRobot {
public:
	static Robot* instance;

	OI* mp_operatorInterface;
	SendableChooser* mp_autonomousModeChooser;
	int ticks;
	std::unique_ptr<Command> mp_driveJoystickCommand;

	Robot();
	virtual ~Robot();

	/**
	 * This method is for things that must always run, whether the robot is disabled, in auto, or in teleop
	 * eg capturing camera images and sending them to the dashboard
	 */
	void AlwaysPeriodic();

	void DisabledInit();
	void RobotInit();
	void DisabledPeriodic();
	void AutonomousInit();
	void AutonomousPeriodic();
	void TeleopInit();
	void TeleopPeriodic();
	void TestPeriodic();
};
