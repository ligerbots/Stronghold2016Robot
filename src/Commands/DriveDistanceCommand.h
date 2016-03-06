#ifndef DriveDistanceCommand_H
#define DriveDistanceCommand_H

#include "../CommandBase.h"
#include "WPILib.h"

class DriveDistanceCommand: public CommandBase
{
private:
	Parameter<double> driveStraightGain;

protected:
	double distance;
	double startPositionLeft;
	double startPositionRight;

	static constexpr double RAMP_RATE = 0.1;
	static constexpr double MAX_SPEED = 0.7;
public:
	explicit DriveDistanceCommand(double distance);
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

#endif
