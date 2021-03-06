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
	mp_position = NULL;
	mp_defense = NULL;
	mp_target = NULL;
}

void FieldInfo::initSelectors(){
	mp_position = new SendableChooser();
	mp_defense = new SendableChooser();
	mp_target = new SendableChooser();

	mp_position->AddDefault("1", &I[POS_ONE]);
	mp_position->AddObject("2", &I[POS_TWO]);
	mp_position->AddObject("3", &I[POS_THREE]);
	mp_position->AddObject("4", &I[POS_FOUR]);
	mp_position->AddObject("5", &I[POS_FIVE]);

	mp_defense->AddDefault("Low Bar", &I[DEF_LOW_BAR]);
	mp_defense->AddObject("A Portcullis", &I[DEF_PORTCULLIS]);
	mp_defense->AddObject("A Cheval de Frise", &I[DEF_CHEVAL]);
	mp_defense->AddObject("B Moat", &I[DEF_MOAT]);
	mp_defense->AddObject("B Ramparts", &I[DEF_RAMPARTS]);
	mp_defense->AddObject("C Drawbridge", &I[DEF_DRAWBRIDGE]);
	mp_defense->AddObject("C Sally Port", &I[DEF_SALLY_PORT]);
	mp_defense->AddObject("D Rock Wall", &I[DEF_ROCK_WALL]);
	mp_defense->AddObject("D Rough Terrain", &I[DEF_ROUGH_TERRAIN]);
	mp_defense->AddObject("NO AUTONOMOUS", &I[DEF_NO_AUTO]);

	mp_target->AddDefault("Left", &I[TARGET_LEFT]);
	mp_target->AddObject("Center", &I[TARGET_CENTER]);
	mp_target->AddObject("Right", &I[TARGET_RIGHT]);
	mp_target->AddObject("ONLY CROSS", &I[TARGET_NONE]);
	SmartDashboard::PutData("Auto Position", mp_position);
	SmartDashboard::PutData("Auto Defense", mp_defense);
	SmartDashboard::PutData("Auto Target", mp_target);
}

// for the next functions, the "line noise"-looking operation is:
// 1. Cast void* from GetSelected() to int*
// 2. Dereference int* to int
int FieldInfo::GetPosition() {
	return *((int*)mp_position->GetSelected());
}

int FieldInfo::GetDefense() {
	return *((int*)mp_defense->GetSelected());
}

int FieldInfo::GetTarget() {
	return *((int*)mp_target->GetSelected());
}

bool FieldInfo::IsCrossingSlowly() {
	return defenseStrategy[*((int*)mp_defense->GetSelected())].speed == SLOW;
}

double FieldInfo::GetInitialOrientation() {
	double orient = defenseStrategy[*((int*)mp_defense->GetSelected())].Orientation;
	return orient;
}


