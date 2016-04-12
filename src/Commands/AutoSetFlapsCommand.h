#pragma once

#include "../CommandBase.h"
#include "WPILib.h"

class AutoSetFlapsCommand: public CommandBase
{
protected:
	double m_flapsFractionToSet;
	bool m_isDifferential;
public:
	AutoSetFlapsCommand();
	explicit AutoSetFlapsCommand(bool isDifferential);
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};
