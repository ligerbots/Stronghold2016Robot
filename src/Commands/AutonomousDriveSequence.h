#pragma once

#include "Commands/CommandGroup.h"
#include "WPILib.h"

class AutonomousDriveSequence: public CommandGroup {
public:
	AutonomousDriveSequence(bool wedgesUp, bool intakeUp, double driveDistance);
};
