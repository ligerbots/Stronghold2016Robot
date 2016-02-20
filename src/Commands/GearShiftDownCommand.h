#pragma once

#include <CommandBase.h>
#include <WPILib.h>

//Basic command to copy

class GearShiftDownCommand: public CommandBase {
protected:

public:
	explicit GearShiftDownCommand();
	void Initialize();
	void Execute();

	bool IsFinished();
	void End();
	void Interrupted();


};
