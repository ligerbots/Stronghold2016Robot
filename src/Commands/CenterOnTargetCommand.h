#pragma once

#include "../CommandBase.h"
#include "WPILib.h"

class CenterOnTargetCommand: public CommandBase {
protected:
	PIDController *mp_softwarePID;
	double m_centerTo;
	Parameter<double> centerMediumZone;
	Parameter<double> centerSlowZone;
	Parameter<double> fastSpeed;
	Parameter<double> mediumSpeed;
	Parameter<double> slowSpeed;
	int m_ticksSinceCentered;
	bool m_isCentered;
public:
	explicit CenterOnTargetCommand();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();

	static constexpr double ACCEPTABLE_ERROR = 0.00625;
};
