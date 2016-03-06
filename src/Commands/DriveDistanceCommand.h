#ifndef DriveDistanceCommand_H
#define DriveDistanceCommand_H

#include "../CommandBase.h"
#include "WPILib.h"

class DriveDistanceCommand: public CommandBase
{
protected:
	double distance;
	double startPositionLeft;
	double startPositionRight;

	static constexpr double RAMP_RATE = 0.1;
	static constexpr double MAX_SPEED = 0.7;
	static constexpr double TICKS_PER_FOOT = 7673; // empirically measured by driving 5 feet
public:
	explicit DriveDistanceCommand(double distance);
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

#endif
