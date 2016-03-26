#ifndef ToggleCommand_H
#define ToggleCommand_H

#include "../CommandBase.h"
#include "WPILib.h"

class ToggleCommand: public CommandBase
{
protected:
	Command* whatToCall;
	std::vector<Command*> ma_restartCommands;
public:
	explicit ToggleCommand(Command* whatToCall);
	ToggleCommand(Command* whatToCall, std::vector<Command*> whatToRestart);
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

#endif
