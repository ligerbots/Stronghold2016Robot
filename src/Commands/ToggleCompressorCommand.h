#pragma once

#include <CommandBase.h>
#include <WPILib.h>

//Basic command to copy

class ToggleCompressorCommand: public CommandBase {
protected:

public:
	explicit ToggleCompressorCommand();
	void Initialize();
	void Execute();

	bool IsFinished();
	void End();
	void Interrupted();


};
