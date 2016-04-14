#pragma once

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

#include <WPILib.h>
//#include "OI.h"

class FieldInfo {
public:
	FieldInfo();
	int GetPosition();
	int GetDefense();
	int GetTarget();
	bool IsCrossingSlowly();
	double GetInitialOrientation();
	void initSelectors();

	SendableChooser *mp_position;
	SendableChooser *mp_defense;
	SendableChooser *mp_target;

	static constexpr double RobotLength = 32.0;

	// The back of our robot must start out touching the center starting tape
	// That tape is 11" from the center of the field
	static constexpr double StartY = 11.0;
	// The distance from the starting tape to the starting ramp of each defense
	// (We've rounded here, it's actually 0.4" longer)
	static constexpr double StartingTapeToDefense = 86.0;
	// How far we drive to get from start position to the start of the first defense
	// (this is drive distance, we end up at a Y offset that's this + StartY)
	static constexpr double StartToDefenseDistance = StartingTapeToDefense - RobotLength;
	// Although the defenses are actually 47.2" wide, one drives the full 48" to get across
	static constexpr double DefenseDepth = 60;
	// After we cross the defense, drive one more foot in autonomous
	static constexpr double DrivePastDefense = 18.0; // +6in

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
	static const std::string StartingPositionNames[];

	struct StartingLocations {
		double x, y;
	};

	static constexpr StartingLocations startingLocations[5] = {
			{-120.0, StartY },
			{ -72.0, StartY },
			{ -24.0, StartY },
			{  24.0, StartY },
			{  72.0, StartY }
	};

	// Target LEFT & RIGHT positions are relative to the direction the robot is facing.
	enum Targets {
		TARGET_LEFT,
		TARGET_CENTER,
		TARGET_RIGHT,
		TARGET_NONE,
		TARGET_MAX
	};
	static const std::string TargetNames[];

	struct TargetLocations {
		double x, y;
	};

	static constexpr TargetLocations targetLocations[] = {
			{ -87.0, 212.0 },
			{ 10.0, 176.0 },
			{ 60.0, 200.0 },
			{ 0, 0 } // target_none
	};

	// angles we need to move to roughly using the navx so that we can see the target
	// in navx angles (clockwise is positive, forward at start of auto is 0)
	static constexpr double targetLineUpAngles[] = {
			60,
			0,
			330,
			0 // target_none
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
		DEF_NO_AUTO,
		DEF_MAX
	};
	static const std::string DefenseNames[];

	enum Speeds {
		NOGO,
		SLOW,
		NORMAL,
		FAST
	};

	// For each defense, list whether we take it wedge side first, or intake side first
	// and what speed
	struct DefenseStrategy {
		double Orientation;
		Speeds speed;
		bool wedgesUp;
	};

	static constexpr DefenseStrategy defenseStrategy[] = {
		{  0.0, FAST,   false}, // DEF_LOW_BAR,
		{  0.0, FAST,   false},	// DEF_PORTCULLIS,
		{  0.0, FAST,   false},	// DEF_CHEVAL,
		{180.0, FAST,   true},	// DEF_MOAT,
		{180.0, FAST,   true},// DEF_RAMPARTS,
		{  0.0, NORMAL, true},	// DEF_DRAWBRIDGE, should we actually just block this choice?
		{  0.0, NORMAL, true},	// DEF_SALLY_PORT, should we actually just block this choice?
		{    0,	FAST,   true},	// DEF_ROCK_WALL,
		{    0, FAST,   true},	// DEF_ROUGH_TERRAIN,
		{  0.0, FAST,   true}	// DEF_MAX (there is no defense max, but just in case)
	};

	struct VisionDataPoint {
		int distance;
		int angle;
		int xpos;
		int ypos;
	};

	static constexpr VisionDataPoint visionData[] = {
			{48, 0, 294, 106},
			{48, 10, 253, 102},
			{48, 20, 214, 93},
			{48, 30, 162, 77},
			{48, 40, 123, 57},
			{48, -10, 342, 105},
			{48, -20, 384, 96},
			{48, -30, 432, 81},
			{48, -40, 482, 53},
			{60, 0, 298, 148},
			{60, -10, 351, 145},
			{60, -20, 406, 133},
			{60, -30, 459, 114},
			{60, -40, 512, 86},
			{60, 10, 249, 152},
			{60, 20, 188, 139},
			{60, 30, 138, 122},
			{60, 40, 77, 89},
			{72, 0, 295, 184},
			{72, 10, 238, 180},
			{72, 20, 172, 168},
			{72, 30, 126, 153},
			{72, 40, 56, 113},
			{72, -10, 353, 181},
			{72, -20, 412, 169},
			{72, -30, 481, 147},
			{72, -40, 540, 117},
			{84, 0, 298, 214},
			{84, 10, 235, 213},
			{84, 20, 170, 201},
			{84, 30, 94, 179},
			{84, -10, 360, 214},
			{84, -20, 419, 205},
			{84, -30, 462, 194},
			{84, -40, 539, 163},
			{96, 0, 294, 240},
			{96, 10, 232, 236},
			{96, 20, 166, 226},
			{96, 30, 96, 209},
			{96, -10, 365, 238},
			{96, -20, 424, 231},
			{96, -30, 491, 218},
			{96, -40, 568, 189},
			{108, 0, 298, 262},
			{108, 10, 233, 260},
			{108, 20, 150, 248},
			{108, 30, 73, 224},
			{108, -10, 363, 260},
			{108, -20, 441, 252},
			{108, -30, 542, 226},
			{120, 0, 303, 284},
			{120, 10, 215, 279},
			{120, 20, 127, 263},
			{120, 30, 46, 238},
			{120, -10, 371, 282},
			{120, -20, 436, 274},
			{120, -30, 510, 259}
	};

	struct VisionFlapDataPoint {
		double distance;
		double fineAngle;
		double leftFlap;
		double rightFlap;
	};

	static constexpr VisionFlapDataPoint differentialFlapData[] = {
			{49.5,  -4.000000, 0.47, 0.54}, // fudged
			{49.5,  -3.113431, 0.47, 0.54},
			{49.5,  -2.066627, 0.50, 0.54},
			{49.5,   0.712809, 0.53, 0.53},
			{49.5,   1.934925, 0.53, 0.50},
			{49.5,   2.934617, 0.50, 0.47},
			{49.5,   4.000000, 0.50, 0.47}, // fudged
			{57.5,  -4.000000, 0.39, 0.49}, // fudged
			{57.5,  -2.891445, 0.39, 0.49},
			{57.5,  -1.903014, 0.40, 0.49},
			{57.5,   0.022209, 0.44, 0.44},
			{57.5,   1.875367, 0.48, 0.40},
			{57.5,   2.772729, 0.49, 0.39},
			{57.5,   4.000000, 0.49, 0.39}, // fudged
			{72.5,  -4.000000, 0.36, 0.44}, // fudged
			{72.5,  -3.161245, 0.36, 0.44},
			{72.5,  -2.277607, 0.39, 0.45},
			{72.5,   0.323484, 0.42, 0.42},
			{72.5,   2.019664, 0.44, 0.38},
			{72.5,   3.659004, 0.44, 0.36},
			{72.5,   4.000000, 0.44, 0.36}, // fudged
			{84.7,  -4.000000, 0.30, 0.42}, // fudged
			{84.7,  -2.875863, 0.30, 0.42},
			{84.7,  -2.050338, 0.32, 0.41},
			{84.7,   0.360515, 0.37, 0.37},
			{84.7,   1.928718, 0.40, 0.33},
			{84.7,   3.123535, 0.41, 0.30},
			{84.7,   4.000000, 0.41, 0.30}, // fudged
			{96.0,  -4.000000, 0.30, 0.40}, // fudged
			{96.0,  -2.175115, 0.30, 0.40},
			{96.0,   0.476888, 0.36, 0.36},
			{96.0,   1.929567, 0.40, 0.34},
			{96.0,   4.000000, 0.40, 0.34}, // fudged
			{107.0, -4.000000, 0.29, 0.40}, // fudged
			{107.0, -2.248067, 0.29, 0.40},
			{107.0,  0.228818, 0.34, 0.34},
			{107.0,  1.965219, 0.38, 0.30},
			{107.0,  4.000000, 0.38, 0.30}, // fudged
			{116.3, -4.000000, 0.31, 0.39}, // fudged
			{116.3, -1.940819, 0.31, 0.39},
			{116.3,  0.086004, 0.35, 0.35},
			{116.3,  2.031037, 0.39, 0.30},
			{116.3,  4.000000, 0.39, 0.30}  // fudged
	};

	static constexpr double flapDataMinDistance = 48.0;
	static constexpr double flapDataMaxDistance = 120.0;
	static constexpr double flapDataMinAngle = -4.0;
	static constexpr double flapDataMaxAngle = 4.0;
};
