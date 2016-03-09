#ifndef AutonomousDriveAndSHoot_H
#define AutonomousDriveAndSHoot_H

#include "Commands/CommandGroup.h"
#include "WPILib.h"

class AutonomousDriveAndShoot: public CommandGroup
{
public:
	AutonomousDriveAndShoot(int position, int defense, int target);
};

#endif
