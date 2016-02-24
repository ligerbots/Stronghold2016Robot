#ifndef GearShiftCommand_H
#define GearShiftCommand_H

#include <CommandBase.h>
#include <WPILib.h>

class GearShiftCommand: public CommandBase {
protected:
	bool isShiftedUp;
public:
	GearShiftCommand();
	void Initialize();
	void Execute();bool IsFinished();
	void End();
	void Interrupted();
};

#endif
