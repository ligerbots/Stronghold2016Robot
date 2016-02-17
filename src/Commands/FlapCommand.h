#pragma once

#include <CommandBase.h>
#include <WPILib.h>

/**
 * Toggles the LED ring around the front camera
 */
class FlapCommand: public CommandBase {
protected:
	int m_ticks;
	double m_position;
	bool done;
public:
	explicit FlapCommand();
	void Initialize();
	void Execute();

	bool IsFinished();
	void End();
	void Interrupted();


};
