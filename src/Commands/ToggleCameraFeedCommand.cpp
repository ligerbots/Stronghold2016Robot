#include <Stronghold2016Robot.h>

ToggleCameraFeedCommand::ToggleCameraFeedCommand() : CommandBase("ToggleCameraFeedCommand"){
}

void ToggleCameraFeedCommand::Initialize() {
}

void ToggleCameraFeedCommand::Execute() {
	printf("ToggleCameraFeedCommand: toggling\n");
	visionSubsystem->toggleCameraFeed();
}

bool ToggleCameraFeedCommand::IsFinished() {
	return true;
}

void ToggleCameraFeedCommand::End() {
}

void ToggleCameraFeedCommand::Interrupted() {
}
