#pragma once

#include <CommandBase.h>
#include <WPILib.h>

/**
 * Toggles the LED ring around the front camera
 */
class LeftFlapTestCommand: public CommandBase {
protected:
	int m_ticks;
	bool done;
public:
	explicit LeftFlapTestCommand();
	void Initialize();
	void Execute();

	bool IsFinished();
	void End();
	void Interrupted();


};
