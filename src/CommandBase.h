#pragma once

#include <Subsystems/DriveSubsystem.h>
#include <Subsystems/VisionSubsystem.h>
#include <Subsystems/NavXSubsystem.h>
#include <Subsystems/ShooterSubsystem.h>
#include <Subsystems/PDPSubsystem.h>
#include <Subsystems/CompressorSubsystem.h>

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
	static std::unique_ptr<ShooterSubsystem> shooterSubsystem;
	static std::unique_ptr<PDPSubsystem> pdpSubsystem;
	static std::unique_ptr<CompressorSubsystem> compressorSubsystem;

	// all commands
	static std::unique_ptr<Command> driveJoystickCommand;
	static std::unique_ptr<Command> ledOnCommand;
	static std::unique_ptr<Command> ledOffCommand;
	static std::unique_ptr<Command> centerOnTargetCommand;
	static std::unique_ptr<Command> leftFlapTestCommand;

};
