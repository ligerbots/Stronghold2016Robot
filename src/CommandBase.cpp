#include <Stronghold2016Robot.h>

std::unique_ptr<VisionSubsystem> CommandBase::visionSubsystem = NULL;
std::unique_ptr<DriveSubsystem> CommandBase::driveSubsystem = NULL;
std::unique_ptr<NavXSubsystem> CommandBase::navXSubsystem = NULL;
std::unique_ptr<ShooterSubsystem> CommandBase::shooterSubsystem = NULL;
std::unique_ptr<FlapSubsystem> CommandBase::flapSubsystem = NULL;
std::unique_ptr<PDPSubsystem> CommandBase::pdpSubsystem = NULL;
std::unique_ptr<CompressorSubsystem> CommandBase::compressorSubsystem = NULL;
std::unique_ptr<WedgeSubsystem> CommandBase::wedgeSubsystem = NULL;
std::unique_ptr<IntakeSubsystem> CommandBase::intakeSubsystem = NULL;

std::unique_ptr<Command> CommandBase::driveJoystickCommand = NULL;
std::unique_ptr<Command> CommandBase::ledOnCommand = NULL;
std::unique_ptr<Command> CommandBase::ledOffCommand = NULL;
std::unique_ptr<Command> CommandBase::centerOnTargetCommand = NULL;
std::unique_ptr<Command> CommandBase::flapCommand = NULL;
std::unique_ptr<Command> CommandBase::tabulaRasaCommand = NULL;
std::unique_ptr<Command> CommandBase::shootCommand = NULL;
std::unique_ptr<Command> CommandBase::toggleCompressorCommand = NULL;
std::unique_ptr<Command> CommandBase::wedgeToggleCommand = NULL;
std::unique_ptr<Command> CommandBase::intakeToggleCommand = NULL;
std::unique_ptr<Command> CommandBase::gearShiftDownCommand = NULL;
std::unique_ptr<Command> CommandBase::gearShiftUpCommand = NULL;

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
	shooterSubsystem.reset(new ShooterSubsystem());
	flapSubsystem.reset(new FlapSubsystem());
	pdpSubsystem.reset(new PDPSubsystem());
	wedgeSubsystem.reset(new WedgeSubsystem());
	intakeSubsystem.reset(new IntakeSubsystem());
	compressorSubsystem.reset(new CompressorSubsystem());

	driveJoystickCommand.reset(new DriveJoystickCommand());
	ledOnCommand.reset(new ToggleLedCommand(true));
	ledOffCommand.reset(new ToggleLedCommand(false));
	centerOnTargetCommand.reset(new CenterOnTargetCommand());
	flapCommand.reset(new FlapCommand());
	tabulaRasaCommand.reset(new TabulaRasaCommand());
	shootCommand.reset(new ShootCommand());
	toggleCompressorCommand.reset(new ToggleCompressorCommand());
	wedgeToggleCommand.reset(new WedgeToggleCommand());
	intakeToggleCommand.reset(new IntakeToggleCommand());
	gearShiftDownCommand.reset(new GearShiftDownCommand());
	gearShiftUpCommand.reset(new GearShiftUpCommand());

}
