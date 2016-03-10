#ifndef RotateIMUCommand_H
#define RotateIMUCommand_H

#include "../CommandBase.h"
#include "WPILib.h"

class RotateIMUCommand: public CommandBase {
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
	explicit RotateIMUCommand(double targetAngle, bool absolute = true);
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

#endif
