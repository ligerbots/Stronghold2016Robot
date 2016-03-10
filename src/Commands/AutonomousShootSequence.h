#ifndef ShootSequence_H
#define ShootSequence_H

#include "Commands/CommandGroup.h"
#include "WPILib.h"

class AutonomousShootSequence: public CommandGroup {
public:
	AutonomousShootSequence();
	void End();
	void Interrupted();
	void cleanup();
};

#endif
