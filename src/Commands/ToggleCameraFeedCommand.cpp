#include <Stronghold2016Robot.h>

ToggleCameraFeedCommand::ToggleCameraFeedCommand() : CommandBase("ToggleCameraFeedCommand"), whichCamera(-1) {
	SetRunWhenDisabled(true);
}

ToggleCameraFeedCommand::ToggleCameraFeedCommand(int whichCamera): CommandBase("ToggleCameraFeedCommand"), whichCamera(whichCamera) {
	SetRunWhenDisabled(true);
}

void ToggleCameraFeedCommand::Initialize() {
}

void ToggleCameraFeedCommand::Execute() {
	printf("ToggleCameraFeedCommand: toggling\n");
	if(whichCamera == -1)
		visionSubsystem->toggleCameraFeed();
	else
		visionSubsystem->setCameraFeed(whichCamera);
}

bool ToggleCameraFeedCommand::IsFinished() {
	return true;
}

void ToggleCameraFeedCommand::End() {
}

void ToggleCameraFeedCommand::Interrupted() {
}
