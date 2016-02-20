#ifndef SRC_COMMANDS_DRIVEJOYSTICKCOMMAND_H_
#define SRC_COMMANDS_DRIVEJOYSTICKCOMMAND_H_

#include <WPILib.h>
#include <CommandBase.h>
#include <Robot.h>

class DriveJoystickCommand: public CommandBase
{
public:
	/**
	 * Clamps the specified value between min and max. Used to enforce TURN_MAX and LINEAR_MAX
	 * @param value The value to clamp
	 * @param min The minimum value of value
	 * @param max The maximum value of value
	 * @return The clamped value
	 */
	static double clampJoystickValue(double value, double min, double max);
	DriveJoystickCommand();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
	/**
	 * The maximum turn rate, as defined by the 2015 robot
	 */
	static double TURN_MAX;
	/**
	 * The maximum roadkill speed, for safety
	 */
	static double LINEAR_MAX;
};

#endif /* SRC_COMMANDS_DRIVEJOYSTICKCOMMAND_H_ */
