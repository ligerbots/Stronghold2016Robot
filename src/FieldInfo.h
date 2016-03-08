#pragma once
// include this file in robot.h once and only once.

/* Ligerbots 2877 Stronghold2016 game
 *
 * Information about where various field elements are so we can autonomously navigate
 * around the field.
 *
 * Coordinate system:
 *  X measures short side of field
 *  Y measures long side of field
 *
 *  In any given competition, the coordinate system is relative to the direction
 *  our robot starts in. So positive Y always points toward the tower we're attacking,
 *  and positive X is always +90 from positive Y.
 *
 *  ALL MEASUREMENTS ARE IN INCHES!
 *
 */
	static constexpr double RobotLength = 32.0;

	// The back of our robot must start out touching the center starting tape
	// That tape is 11" from the center of the field
	static constexpr double StartX = 11.0;
	// The distance from the starting tape to the starting ramp of each defense
	// (We've rounded here, it's actually 0.4" longer)
	static constexpr double StartingTapeToDefense = 86.0;
	// How far we drive to get from start position to the start of the first defense
	static constexpr double StartToDefenseDistance = StartingTapeToDefense - (RobotLength + StartX);
	// After we cross the defense, drive one more foot in autonomous
	static constexpr double DrivePastDefense = 12.0;

	// Identity array so we can pass pointers to our enum values
	static int I[32];

	enum StartingPositions {
		POS_ONE,
		POS_TWO,
		POS_THREE,
		POS_FOUR,
		POS_FIVE,
		POS_MAX
	};

	struct StartingLocations {
		double x, y;
	};

	static constexpr StartingLocations startingLocations[] = {
			{-120.0, StartX },
			{ -72.0, StartX },
			{ -24.0, StartX },
			{  24.0, StartX },
			{  48.0, StartX }
	};

	// Target LEFT & RIGHT positions are relative to the direction the robot is facing.
	enum Targets {
		TARGET_LEFT,
		TARGET_CENTER,
		TARGET_RIGHT,
		TARGET_MAX
	};

	struct TargetLocations {
		double x, y;
	};

	static constexpr TargetLocations startlocations[] = {
			{ -75.0, 275.00 },
			{ 10.0, 225.0 },
			{ 50.0, 110.0 }
	};

	enum Defenses {
		DEF_LOW_BAR,
		DEF_PORTCULLIS,
		DEF_CHEVAL,
		DEF_MOAT,
		DEF_RAMPARTS,
		DEF_DRAWBRIDGE,
		DEF_SALLY_PORT,
		DEF_ROCK_WALL,
		DEF_ROUGH_TERRAIN,
		DEF_MAX
	};

