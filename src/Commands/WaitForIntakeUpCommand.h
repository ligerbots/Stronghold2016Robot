#ifndef WaitForIntakeUp_H
#define WaitForIntakeUp_H

#include "../CommandBase.h"
#include "WPILib.h"

class WaitForIntakeUpCommand: public CommandBase {
public:
	WaitForIntakeUpCommand();
	void Initialize();
	void Execute();bool IsFinished();
	void End();
	void Interrupted();
};

#endif
