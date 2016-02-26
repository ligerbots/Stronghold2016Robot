#ifndef RotateIMUCommand_H
#define RotateIMUCommand_H

#include "../CommandBase.h"
#include "WPILib.h"

class RotateIMUCommand: public CommandBase {
protected:
	double targetAngle;
	double currentAngle;
	double lastAngle;
	bool isClockwise;

	void updateCurrentAngle();
public:
	explicit RotateIMUCommand(double targetAngle);
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

#endif
