#include <Stronghold2016Robot.h>

std::unique_ptr<VisionSubsystem> CommandBase::visionSubsystem = NULL;
std::unique_ptr<DriveTrain> CommandBase::driveSubsystem = NULL;

CommandBase::CommandBase(const std::string &name) :
		Command(name) {
}

CommandBase::CommandBase() :
		Command() {
}

void CommandBase::init() {
	driveSubsystem.reset(new DriveTrain());
	visionSubsystem.reset(new VisionSubsystem());
}
