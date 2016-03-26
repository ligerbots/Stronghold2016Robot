#ifndef RollBallPickupThenShooter_H
#define RollBallPickupThenShooter_H

#include "Commands/CommandGroup.h"
#include "WPILib.h"

class RollBallPickupThenShooter: public CommandGroup
{
public:
	RollBallPickupThenShooter();
	void End();
};

#endif
