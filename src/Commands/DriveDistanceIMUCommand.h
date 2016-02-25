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
	double startX;
	double startY;
public:
	explicit DriveDistanceIMUCommand(double distanceMeters);
	void Initialize();
	void Execute();bool IsFinished();
	void End();
	void Interrupted();
};

#endif
