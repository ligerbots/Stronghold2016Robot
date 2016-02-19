#pragma once

#include <CommandBase.h>
#include <WPILib.h>

/**
 * Toggles the LED ring around the front camera
 */
class FlapCommand: public CommandBase {
protected:
	bool m_direction;
	bool m_done;
public:
	Parameter<double> leftLowLimit;
	Parameter<double> leftHighLimit;
	Parameter<double> rightLowLimit;
	Parameter<double> rightHighLimit;

	explicit FlapCommand();
	void Initialize();
	void Execute();

	bool IsFinished();
	void End();
	void Interrupted();


};
