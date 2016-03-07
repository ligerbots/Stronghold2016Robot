#pragma once

#include <CommandBase.h>
#include <WPILib.h>

//Basic command to copy

class WedgeToggleCommand: public CommandBase {
private:
	int action;
public:
	WedgeToggleCommand();
	explicit WedgeToggleCommand(bool wedgeUp);
	void Initialize();
	void Execute();

	bool IsFinished();
	void End();
	void Interrupted();


};
