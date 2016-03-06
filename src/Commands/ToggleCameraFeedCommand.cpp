#include <Stronghold2016Robot.h>

ToggleCameraFeedCommand::ToggleCameraFeedCommand() : CommandBase("ToggleCameraFeedCommand"), whichCamera(-1) {
}

ToggleCameraFeedCommand::ToggleCameraFeedCommand(int whichCamera): CommandBase("ToggleCameraFeedCommand"), whichCamera(whichCamera) {
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
