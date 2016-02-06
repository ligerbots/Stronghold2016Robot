#include <Stronghold2016Robot.h>

std::unique_ptr<VisionSubsystem> CommandBase::visionSubsystem = NULL;
std::unique_ptr<DriveSubsystem> CommandBase::driveSubsystem = NULL;

std::unique_ptr<Command> CommandBase::driveJoystickCommand = NULL;
std::unique_ptr<Command> CommandBase::ledOnCommand = NULL;
std::unique_ptr<Command> CommandBase::ledOffCommand = NULL;

CommandBase::CommandBase(const std::string &name) :
		Command(name) {
}

CommandBase::CommandBase() :
		Command() {
}

void CommandBase::init() {
	driveSubsystem.reset(new DriveSubsystem());
	visionSubsystem.reset(new VisionSubsystem());

	driveJoystickCommand.reset(new DriveJoystickCommand());
	ledOnCommand.reset(new ToggleLedCommand(true));
	ledOffCommand.reset(new ToggleLedCommand(false));
}
