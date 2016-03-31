#include <Stronghold2016Robot.h>

ToggleCameraFeedCommand::ToggleCameraFeedCommand() : CommandBase("ToggleCameraFeedCommand"), m_whichCamera(-1) {
	SetRunWhenDisabled(true); // doesn't really work
}

ToggleCameraFeedCommand::ToggleCameraFeedCommand(int whichCamera): CommandBase("ToggleCameraFeedCommand"), m_whichCamera(whichCamera) {
	SetRunWhenDisabled(true);
}

void ToggleCameraFeedCommand::Initialize() {
}

void ToggleCameraFeedCommand::Execute() {
	printf("ToggleCameraFeedCommand: toggling\n");
	if(m_whichCamera == -1) { // mode: toggle to next camera
		visionSubsystem->toggleCameraFeed();
	} else { // mode: toggle to a specific camera
		visionSubsystem->setCameraFeed(m_whichCamera);
	}
}

bool ToggleCameraFeedCommand::IsFinished() {
	return true;
}

void ToggleCameraFeedCommand::End() {
}

void ToggleCameraFeedCommand::Interrupted() {
}
