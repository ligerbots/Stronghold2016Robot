#ifndef ToggleLedCommand_H
#define ToggleLedCommand_H

#include <CommandBase.h>
#include <WPILib.h>

/**
 * Toggles the LED ring around the front camera
 */
class ToggleLedCommand: public CommandBase {
protected:
public:
	explicit ToggleLedCommand();
	void Initialize();
	void Execute();

	bool IsFinished();
	void End();
	void Interrupted();
};

#endif
