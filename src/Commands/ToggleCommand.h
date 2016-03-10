#ifndef ToggleCommand_H
#define ToggleCommand_H

#include "../CommandBase.h"
#include "WPILib.h"

class ToggleCommand: public CommandBase
{
protected:
	Command* whatToCall;
public:
	explicit ToggleCommand(Command* whatToCall);
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

#endif
