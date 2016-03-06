#ifndef IntakeRollerCommand_H
#define IntakeRollerCommand_H

#include "../CommandBase.h"
#include "WPILib.h"

class IntakeRollerCommand: public CommandBase
{
private:
	Robot *robot;		// cache Robot::instance
public:
	IntakeRollerCommand();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

#endif
