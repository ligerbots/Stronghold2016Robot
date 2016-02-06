#ifndef ToggleLedCommand_H
#define ToggleLedCommand_H

#include <CommandBase.h>
#include <WPILib.h>

class ToggleLedCommand: public CommandBase {
protected:
	bool on;
public:
	explicit ToggleLedCommand(bool on);
	void Initialize();
	void Execute();

	bool IsFinished();
	void End();
	void Interrupted();
};

#endif
