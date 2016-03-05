#ifndef GearShiftCommand_H
#define GearShiftCommand_H

#include <CommandBase.h>
#include <WPILib.h>

class GearShiftCommand: public CommandBase {
public:
	enum Direction {
		SHIFT_DOWN = -1,
		TOGGLE = 0,
		SHIFT_UP = 1
	};
	explicit GearShiftCommand(Direction direction);
	void Initialize();
	void Execute();bool IsFinished();
	void End();
	void Interrupted();
	// m_direction specifies which type of shift command this is
	// an instance of (toggle, shift up, or shift down)
	Direction m_direction;
};

#endif
