#ifndef DriveDistanceIMUCommand_H
#define DriveDistanceIMUCommand_H

#include "../CommandBase.h"
#include "WPILib.h"

/**
 * Like DriveDistanceCommand, but uses the NavX
 */
class DriveDistanceIMUCommand: public CommandBase {
protected:
	double distanceMeters2;
	float startX;
	float startY;
	float startZ;
	double speed;
public:
	explicit DriveDistanceIMUCommand(double distanceMeters, double speed = 1.0);
	void Initialize();
	void Execute();bool IsFinished();
	void End();
	void Interrupted();
};

#endif
