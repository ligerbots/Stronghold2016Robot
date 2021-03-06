#pragma once

#include <Subsystems/DriveSubsystem.h>
#include <Subsystems/VisionSubsystem.h>
#include <Subsystems/NavXSubsystem.h>
#include <Subsystems/ShooterSubsystem.h>
#include <Subsystems/FlapSubsystem.h>
#include <Subsystems/PDPSubsystem.h>
#include <Subsystems/CompressorSubsystem.h>
#include <Subsystems/WedgeSubsystem.h>
#include <Subsystems/IntakeSubsystem.h>
#include <Subsystems/DummySubsystem.h>

/**
 * The base for most parameter-less commands. All atomic commands should subclass CommandBase.
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
	static std::unique_ptr<FlapSubsystem> flapSubsystem;
	static std::unique_ptr<PDPSubsystem> pdpSubsystem;
	static std::unique_ptr<CompressorSubsystem> compressorSubsystem;
	static std::unique_ptr<WedgeSubsystem> wedgeSubsystem;
	static std::unique_ptr<IntakeSubsystem> intakeSubsystem;
	static std::unique_ptr<DummySubsystem> dummySubsystem;

	// commands that require a static instance
	static std::unique_ptr<Command> driveJoystickCommand;
	static std::unique_ptr<Command> centerOnTargetCommand;
	static std::unique_ptr<Command> flapCommand;
	static std::unique_ptr<Command> tabulaRasaCommand;
	static std::unique_ptr<Command> shootCommand;
	static std::unique_ptr<Command> toggleCompressorCommand;
	static std::unique_ptr<Command> toggleLedCommand;
	static std::unique_ptr<Command> wedgeToggleCommand;
	static std::unique_ptr<Command> intakeToggleCommand;
	static std::unique_ptr<Command> gearShiftDownCommand;
	static std::unique_ptr<Command> gearShiftUpCommand;
	static std::unique_ptr<Command> intakeRollerCommand;
	static std::unique_ptr<Command> gearShiftCommand;
	static std::unique_ptr<Command> toggleCameraFeedCommand;
	static std::unique_ptr<Command> autoSetFlapsCommand;
	static std::unique_ptr<Command> diffShootCommand;
};
