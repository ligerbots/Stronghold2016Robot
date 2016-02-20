#pragma once

#include <CommandBase.h>
#include <WPILib.h>

//Basic command to copy

class WedgeToggleCommand: public CommandBase {
private:
	bool wedgeDown;
public:
	explicit WedgeToggleCommand();
	void Initialize();
	void Execute();

	bool IsFinished();
	void End();
	void Interrupted();


};
