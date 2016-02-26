#ifndef WaitForIntakeUp_H
#define WaitForIntakeUp_H

#include "../CommandBase.h"
#include "WPILib.h"

class WaitForIntakeUp: public CommandBase {
public:
	WaitForIntakeUp();
	void Initialize();
	void Execute();bool IsFinished();
	void End();
	void Interrupted();
};

#endif
