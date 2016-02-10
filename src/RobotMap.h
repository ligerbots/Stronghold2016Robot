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

	static constexpr int LED_RING_SPIKE = 0;

	// left = 1, 3, 5
	// right = 2, 4, 6
	static constexpr int CT_DRIVE_LEFT1 = 1;
	static constexpr int CT_DRIVE_LEFT2 = 3;
	static constexpr int CT_DRIVE_LEFT3 = 5;
	static constexpr int CT_DRIVE_RIGHT1 = 2;
	static constexpr int CT_DRIVE_RIGHT2 = 4;
	static constexpr int CT_DRIVE_RIGHT3 = 6;

	static constexpr int PCM_COMPRESSOR = 7;
};
