#pragma once

#include <CommandBase.h>
#include <WPILib.h>

//Basic command to copy

class IntakeToggleCommand: public CommandBase {
private:
	int m_mode;
public:
	explicit IntakeToggleCommand();
	explicit IntakeToggleCommand(bool intakeUp);
	void Initialize();
	void Execute();

	bool IsFinished();
	void End();
	void Interrupted();


};
