#ifndef RollBallPickupThenShooter_H
#define RollBallPickupThenShooter_H

#include "Commands/CommandGroup.h"
#include "WPILib.h"

class RollBallPickupThenShooter: public CommandGroup
{
protected:
	bool m_alreadyInShooter;
public:
	RollBallPickupThenShooter();
	void Initialize();
	bool IsFinished();
	void End();
};

#endif
