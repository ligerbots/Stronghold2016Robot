#include <Stronghold2016Robot.h>

std::unique_ptr<VisionSubsystem> CommandBase::visionSubsystem = NULL;
std::unique_ptr<DriveSubsystem> CommandBase::driveSubsystem = NULL;
std::unique_ptr<NavXSubsystem> CommandBase::navXSubsystem = NULL;
std::unique_ptr<ShooterSubsystem> CommandBase::shooterSubystem = NULL;
std::unique_ptr<PDPSubsystem> CommandBase::pdpSubystem = NULL;
std::unique_ptr<CompressorSubsystem> CommandBase::compressorSubystem = NULL;

std::unique_ptr<Command> CommandBase::driveJoystickCommand = NULL;
std::unique_ptr<Command> CommandBase::ledOnCommand = NULL;
std::unique_ptr<Command> CommandBase::ledOffCommand = NULL;
std::unique_ptr<Command> CommandBase::centerOnTargetCommand = NULL;

CommandBase::CommandBase(const std::string &name) :
		Command(name) {
}

CommandBase::CommandBase() :
		Command() {
}

void CommandBase::init() {
	driveSubsystem.reset(new DriveSubsystem());
	visionSubsystem.reset(new VisionSubsystem());
	navXSubsystem.reset(new NavXSubsystem());
	shooterSubystem.reset(new ShooterSubsystem());
	pdpSubystem.reset(new PDPSubsystem());
//	compressorSubystem.reset(new CompressorSubsystem());

	driveJoystickCommand.reset(new DriveJoystickCommand());
	ledOnCommand.reset(new ToggleLedCommand(true));
	ledOffCommand.reset(new ToggleLedCommand(false));
	centerOnTargetCommand.reset(new CenterOnTargetCommand());
}
