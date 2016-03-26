#ifndef AcquireTarget_H
#define AcquireTarget_H

#include "../CommandBase.h"
#include "WPILib.h"

class AcquireTarget: public CommandBase
{
protected:
	bool m_waitForVision;
	bool m_forceUpdate;
	// calls visionSubsystem->isVisionCalculationDirty()
	// if that returns false, then it skips getting a frame
	bool m_isRunningVision;
public:
	AcquireTarget(bool waitForVision = false, bool forceUpdate = false);
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

#endif
