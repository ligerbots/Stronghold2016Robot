#pragma once

#include "../CommandBase.h"
#include "WPILib.h"

class AutonomousShootCommand: public CommandBase {
protected:
	int m_ticks;
	double flapAngle;
	bool isSafe;
public:
	AutonomousShootCommand();
	void Initialize();
	void Execute();bool IsFinished();
	void End();
	void Interrupted();
};
