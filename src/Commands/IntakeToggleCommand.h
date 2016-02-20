#pragma once

#include <CommandBase.h>
#include <WPILib.h>

//Basic command to copy

class IntakeToggleCommand: public CommandBase {
private:
	bool intakeDown;
public:
	explicit IntakeToggleCommand();
	void Initialize();
	void Execute();

	bool IsFinished();
	void End();
	void Interrupted();


};
