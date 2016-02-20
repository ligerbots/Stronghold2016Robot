#pragma once

#include <CommandBase.h>
#include <WPILib.h>

//Basic command to copy

class GearShiftUpCommand: public CommandBase {
protected:

public:
	explicit GearShiftUpCommand();
	void Initialize();
	void Execute();

	bool IsFinished();
	void End();
	void Interrupted();


};
