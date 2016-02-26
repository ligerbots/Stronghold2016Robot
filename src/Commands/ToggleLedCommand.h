#ifndef ToggleLedCommand_H
#define ToggleLedCommand_H

#include <CommandBase.h>
#include <WPILib.h>

/**
 * Toggles the LED ring around the front camera
 */
class ToggleLedCommand: public CommandBase {
protected:
	bool turnOn;
	bool toggle;
public:
	ToggleLedCommand();
	explicit ToggleLedCommand(bool on);
	void Initialize();
	void Execute();

	bool IsFinished();
	void End();
	void Interrupted();
};

#endif
