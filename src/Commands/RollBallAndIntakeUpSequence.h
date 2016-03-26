#ifndef CenterAndRollBallSequence_H
#define CenterAndRollBallSequence_H

#include "Commands/CommandGroup.h"
#include "WPILib.h"

class RollBallAndIntakeUpSequence: public CommandGroup
{
public:
	RollBallAndIntakeUpSequence();
	void cleanup();
	void End();
	void Interrupted();
};

#endif
