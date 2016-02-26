#pragma once

#include <CommandBase.h>
#include <WPILib.h>

/**
 * Toggles the LED ring around the front camera
 */
class FlapCommand: public CommandBase {
protected:
public:
	Parameter<double> flapPosition;

	explicit FlapCommand();
	void Initialize();
	void Execute();

	bool IsFinished();
	void End();
	void Interrupted();


};
