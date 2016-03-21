#pragma once
#include "../CommandBase.h"
#include "WPILib.h"

class RotateToTarget: public CommandBase {
protected:
	double angle;
	double targetAngle;
	double currentAngle;
	double lastAngle;
	bool isClockwise;
	bool isAbsolute;
	int m_ticks;

	void updateCurrentAngle();
public:
	explicit RotateToTarget();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

