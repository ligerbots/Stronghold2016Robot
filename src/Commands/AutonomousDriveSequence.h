#pragma once

#include "Commands/CommandGroup.h"
#include "WPILib.h"

class AutonomousDriveSequence: public CommandGroup {
public:
	AutonomousDriveSequence(int position, int defense, int target, bool slow = false);
};
