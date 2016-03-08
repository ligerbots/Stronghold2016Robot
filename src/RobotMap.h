#pragma once

#include "WPILib.h"

/**
 * The RobotMap is a mapping from the ports sensors and actuators are wired into
 * to a variable name. This provides flexibility changing wiring, makes checking
 * the wiring easier and significantly reduces the number of magic numbers
 * floating around.
 */
 
// For example to map the left and right motors, you could define the
// following variables to use with your drivetrain subsystem.
//const int LEFTMOTOR = 1;
//const int RIGHTMOTOR = 2;

// If you are using multiple modules, make sure to define both the port
// number and the module. For example you with a rangefinder:
//const int RANGE_FINDER_PORT = 1;
//const int RANGE_FINDER_MODULE = 1;

class RobotMap {
public:

	static constexpr double TICKS_PER_SECOND = 50.0;

	static constexpr int RELAY_LED_RING_SPIKE = 0;

	static constexpr int PDP_CAN = 0;

	// left = 1, 3, 5
	// right = 2, 4, 6
	static constexpr int CT_DRIVE_LEFT1 = 1;
	static constexpr int CT_DRIVE_LEFT2 = 3;
	static constexpr int CT_DRIVE_LEFT3 = 5;
	static constexpr int CT_DRIVE_RIGHT1 = 2;
	static constexpr int CT_DRIVE_RIGHT2 = 4;
	static constexpr int CT_DRIVE_RIGHT3 = 6;

	static constexpr int PCM_CAN = 7;

	static constexpr int CT_INTAKE_ROLLER = 8;

	static constexpr int PCM_HIGH_FLOW_CAN = 9;


	static constexpr int PCM_SHIFTER_LOW_GEAR = 1;
	static constexpr int PCM_SHIFTER_HIGH_GEAR = 0;
	static constexpr int PCM_INTAKE_UP = 4;
	static constexpr int PCM_INTAKE_DOWN = 5;
	static constexpr int PCM_WEDGES_UP = 2;
	static constexpr int PCM_WEDGES_DOWN = 3;

	static constexpr int PCM_SHOOTER_FIRE = 6;
	static constexpr int PCM_SHOOTER_RETRACT = 7;

	static constexpr int LIMIT_SWITCH_INTAKE_UP = 0;
	static constexpr int LIMIT_SWITCH_WEGDE_DOWN = 1;
	static constexpr int LIMIT_SWITCH_INTAKE_BALL_IN_SHOOTER = 2;
	static constexpr int LIMIT_SWITCH_INTAKE_BALL_DEFENSES_POSITION = 3;

	static constexpr int PWM_SERVO_SHOOTER_LEFT = 0;
	static constexpr int PWM_SERVO_SHOOTER_RIGHT = 1;

	// Maximum pitch angle. If the robot gets to this angle, we need to go
	// backwards to keep the robot from tipping over.
	static constexpr float MAX_PITCH_ANGLE = 45.0;
};
