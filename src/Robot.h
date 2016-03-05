#pragma once

#include <WPILib.h>
#include "OI.h"

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

	static int POS_ONE;
	static int POS_TWO;
	static int POS_THREE;
	static int POS_FOUR;
	static int POS_FIVE;

	static int DEF_LOW_BAR;
	static int DEF_PORTCULLIS;
	static int DEF_CHEVAL;
	static int DEF_MOAT;
	static int DEF_RAMPARTS;
	static int DEF_DRAWBRIDGE;
	static int DEF_SALLY_PORT;
	static int DEF_ROCK_WALL;
	static int DEF_ROUGH_TERRAIN;

	static int TARGET_LEFT;
	static int TARGET_CENTER;
	static int TARGET_RIGHT;

	SendableChooser* mp_position;
	SendableChooser* mp_defense;
	SendableChooser* mp_target;

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
