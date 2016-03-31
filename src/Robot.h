#pragma once

#include <WPILib.h>
#include "OI.h"
#include <FieldInfo.h>

class Robot: public IterativeRobot {
public:
	static Robot* instance;
	static int ticks;		// make this globally available
	static timespec time_resolution;
	/**
	 * True if the program is currently running on roadkill
	 * Only useful after CommandBase::init()
	 */
	static bool is_roadkill;

	I2C m_ledTeensyCommunication;
	enum LedState {
		OFF, // all LEDs off
		NORMAL, // detect mode auto, blue alliance, or red alliance and set mode accordingly
		SHOOT // play shoot animation
	};
	void SetLeds(LedState state);

	int m_startTicks;
	timespec m_startSpec;
	double m_startTime;
	//timeval lastLoopRunTime;

	OI* mp_operatorInterface;

	FieldInfo m_fieldInfo;

	Command* mp_autonomousCommand;

	static bool robot_is_about_to_tip;

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
	static double GetRTC();
};
