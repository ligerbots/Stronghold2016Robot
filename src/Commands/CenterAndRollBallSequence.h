#ifndef CenterAndRollBallSequence_H
#define CenterAndRollBallSequence_H

#include "Commands/CommandGroup.h"
#include "WPILib.h"

class CenterAndRollBallSequence: public CommandGroup
{
public:
	CenterAndRollBallSequence();
	void cleanup();
	void End();
	void Interrupted();
};

#endif
