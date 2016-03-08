#pragma once

#include "../CommandBase.h"
#include "WPILib.h"

class CenterOnTargetCommand: public CommandBase {
protected:
	PIDController *mp_softwarePID;
	double centerTo;
	Parameter<double> centerMediumZone;
	Parameter<double> centerSlowZone;
	Parameter<double> fastSpeed;
	Parameter<double> mediumSpeed;
	Parameter<double> slowSpeed;
public:
	explicit CenterOnTargetCommand();
	void Initialize();
	void Execute();bool IsFinished();
	void End();
	void Interrupted();
};
