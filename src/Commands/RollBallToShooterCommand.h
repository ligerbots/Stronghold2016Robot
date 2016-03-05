#pragma once

#include "../CommandBase.h"
#include "WPILib.h"

class RollBallToShooterCommand: public CommandBase {
public:
	RollBallToShooterCommand();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};
