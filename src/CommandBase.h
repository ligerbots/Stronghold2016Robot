#pragma once

#include <Subsystems/DriveTrain.h>
#include <Subsystems/VisionSubsystem.h>

/**
 * The base for all commands. All atomic commands should subclass CommandBase.
 * CommandBase stores creates and stores each control system. To access a
 * subsystem elsewhere in your code in your code use CommandBase.examplesubsystem
 */
class CommandBase: public Command
{
public:
	explicit CommandBase(const std::string &name);
	CommandBase();
	static void init();

	// all subsystems
	static std::unique_ptr<DriveTrain> driveSubsystem;
	static std::unique_ptr<VisionSubsystem> visionSubsystem;

	// all commands
	static std::unique_ptr<Command> driveJoystickCommand;
	static std::unique_ptr<Command> ledOnCommand;
	static std::unique_ptr<Command> ledOffCommand;
};
