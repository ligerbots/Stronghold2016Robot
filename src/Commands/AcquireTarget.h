#ifndef AcquireTarget_H
#define AcquireTarget_H

#include "../CommandBase.h"
#include "WPILib.h"

class AcquireTarget: public CommandBase
{
public:
	AcquireTarget();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

#endif
