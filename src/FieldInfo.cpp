// include this file in robot.h once and only once.
#include "FieldInfo.h"

int FieldInfo::I[];		// our identity array
constexpr FieldInfo::StartingLocations FieldInfo::startingLocations[];
constexpr FieldInfo::TargetLocations FieldInfo::targetLocations[];
constexpr double FieldInfo::targetLineUpAngles[];
constexpr FieldInfo::DefenseStrategy FieldInfo::defenseStrategy[];
constexpr FieldInfo::VisionDataPoint FieldInfo::visionData[];


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
	mp_defense->AddObject("Portcullis", &I[DEF_PORTCULLIS]);
	mp_defense->AddObject("Cheval de Frise", &I[DEF_CHEVAL]);
	mp_defense->AddObject("Moat", &I[DEF_MOAT]);
	mp_defense->AddObject("Ramparts", &I[DEF_RAMPARTS]);
	mp_defense->AddObject("Drawbridge", &I[DEF_DRAWBRIDGE]);
	mp_defense->AddObject("Sally Port", &I[DEF_SALLY_PORT]);
	mp_defense->AddObject("Rock Wall", &I[DEF_ROCK_WALL]);
	mp_defense->AddObject("Rough Terrain", &I[DEF_ROUGH_TERRAIN]);
	mp_defense->AddObject("NO AUTONOMOUS", &I[DEF_NO_AUTO]);

	mp_target->AddDefault("Left", &I[TARGET_LEFT]);
	mp_target->AddObject("Center", &I[TARGET_CENTER]);
	mp_target->AddObject("Right", &I[TARGET_RIGHT]);
	SmartDashboard::PutData("Auto Position", mp_position);
	SmartDashboard::PutData("Auto Defense", mp_defense);
	SmartDashboard::PutData("Auto Target", mp_target);
}

int FieldInfo::GetPosition() {
	return *((int*)mp_position->GetSelected());
}

int FieldInfo::GetDefense() {
	return *((int*)mp_defense->GetSelected());
}

int FieldInfo::GetTarget() {
	return *((int*)mp_target->GetSelected());
}

bool FieldInfo::CrossSlowly() {
	return defenseStrategy[*((int*)mp_defense->GetSelected())].speed == SLOW;
}

double FieldInfo::GetInitialOrientation() {
	double orient = defenseStrategy[*((int*)mp_defense->GetSelected())].Orientation;
	return orient;
}


