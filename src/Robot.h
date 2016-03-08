#pragma once

#include <WPILib.h>
#include "OI.h"
#include <FieldInfo.h>

class Robot: public IterativeRobot {
public:
	static Robot* instance;
	static int ticks;		// make this globally available
	/**
	 * True if the program is currently running on roadkill
	 * Only useful after CommandBase::init()
	 */
	static bool isRoadkill;

	OI* mp_operatorInterface;

	FieldInfo fieldInfo;

	Command* mp_autonomousCommand;

	static bool ROBOT_IS_ABOUT_TO_TIP;

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
