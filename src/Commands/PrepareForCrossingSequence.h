#pragma once

#include "Commands/CommandGroup.h"
#include "WPILib.h"

class PrepareForCrossingSequence: public CommandGroup
{
public:
	PrepareForCrossingSequence();
	void Initialize();
	void Interrupted();
	void End();
	void Cleanup();
};
