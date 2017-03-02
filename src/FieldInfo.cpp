// include this file in robot.h once and only once.
#include "FieldInfo.h"

int FieldInfo::I[];		// our identity array
constexpr FieldInfo::StartingLocations FieldInfo::startingLocations[];
constexpr FieldInfo::TargetLocations FieldInfo::targetLocations[];
constexpr double FieldInfo::targetLineUpAngles[];
constexpr FieldInfo::DefenseStrategy FieldInfo::defenseStrategy[];
constexpr FieldInfo::VisionDataPoint FieldInfo::visionData[];
constexpr FieldInfo::VisionFlapDataPoint FieldInfo::differentialFlapData[];


const std::string FieldInfo::StartingPositionNames[] = {
		"POS_ONE",
		"POS_TWO",
		"POS_THREE",
		"POS_FOUR",
		"POS_FIVE",
		"POS_MAX"
	};
const std::string FieldInfo::DefenseNames[] = {
		"TARGET_LEFT",
		"TARGET_CENTER",
		"TARGET_RIGHT",
		"TARGET_NONE",
		"TARGET_MAX"
	};
const std::string FieldInfo::TargetNames[] = {
		"DEF_LOW_BAR",
		"DEF_PORTCULLIS",
		"DEF_CHEVAL",
		"DEF_MOAT",
		"DEF_RAMPARTS",
		"DEF_DRAWBRIDGE",
		"DEF_SALLY_PORT",
		"DEF_ROCK_WALL",
		"DEF_ROUGH_TERRAIN",
		"DEF_NO_AUTO",
		"DEF_MAX"
	};

FieldInfo::FieldInfo() {
	// initialize our identity array
	for (int i=0; i!=sizeof(I)/sizeof(int); i++) I[i] = i;

}

void FieldInfo::initSelectors(){

}

// for the next functions, the "line noise"-looking operation is:
// 1. Cast void* from GetSelected() to int*
// 2. Dereference int* to int
int FieldInfo::GetPosition() {
	return 0;
}

int FieldInfo::GetDefense() {
	return 0;
}

int FieldInfo::GetTarget() {
	return 0;
}

bool FieldInfo::IsCrossingSlowly() {
	return defenseStrategy[0].speed == SLOW;
}

double FieldInfo::GetInitialOrientation() {
	double orient = defenseStrategy[0].Orientation;
	return orient;
}


