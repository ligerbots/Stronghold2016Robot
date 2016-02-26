#pragma once

#include <CommandBase.h>
#include <WPILib.h>

//Basic command to copy

class ToggleCompressorCommand: public CommandBase {
protected:
	int mode; // 0 - off, 1 - on, 2 - toggle
public:
	ToggleCompressorCommand();
	explicit ToggleCompressorCommand(bool on);
	void Initialize();
	void Execute();

	bool IsFinished();
	void End();
	void Interrupted();


};
