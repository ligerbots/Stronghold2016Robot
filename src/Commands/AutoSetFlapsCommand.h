#pragma once

#include "../CommandBase.h"
#include "WPILib.h"

class AutoSetFlapsCommand: public CommandBase
{
protected:
	double flapsFractionToSet;
public:
	AutoSetFlapsCommand();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};
