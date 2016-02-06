#pragma once

#include <Subsystems/DriveSubsystem.h>
#include <Subsystems/VisionSubsystem.h>
#include <Subsystems/NavXSubsystem.h>

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
	static std::unique_ptr<DriveSubsystem> driveSubsystem;
	static std::unique_ptr<VisionSubsystem> visionSubsystem;
	static std::unique_ptr<NavXSubsystem> navXSubsystem;

	// all commands
	static std::unique_ptr<Command> driveJoystickCommand;
	static std::unique_ptr<Command> ledOnCommand;
	static std::unique_ptr<Command> ledOffCommand;
};
