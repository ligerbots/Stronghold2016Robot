#pragma once

#include "../CommandBase.h"
#include "WPILib.h"

class CenterOnTargetCommand: public CommandBase {
protected:
	PIDController *m_softwarePID;

public:
	CenterOnTargetCommand();
	void Initialize();
	void Execute();bool IsFinished();
	void End();
	void Interrupted();
};
