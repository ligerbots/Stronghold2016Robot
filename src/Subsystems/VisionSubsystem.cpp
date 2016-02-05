#include <Stronghold2016Robot.h>

VisionSubsystem::VisionSubsystem() :
		Subsystem("VisionSubsystem"), exposure("VisionSubsystem_exposure"), runVision(
				"VisionSubsystem_runProcessing"), frameCenterX(0) {
}

void VisionSubsystem::InitDefaultCommand() {
	// TODO: led toggle command
}

void VisionSubsystem::camerasOn() {
	printf("VisionSubsystem: camerasOn\n");
	Camera::EnumerateCameras();
	Camera::EnableCameras();
}

void VisionSubsystem::updateVision(int ticks) {
	// run every second tick
	if (ticks % 2 == 0)
		return;

	if (Camera::GetNumberOfCameras() < 1)
		return;

	Camera::GetCamera(0)->SetExposure(exposure.get());
	Camera::Feed(ticks);

	Image* image = Camera::GetCamera(0)->GetStoredFrame();

	if (runVision.get()) {
		IVA_ProcessImage(image);

		int numParticles;
		bool needsConnection = true;
		imaqCountParticles(image, needsConnection, &numParticles);
		if (numParticles != 0) {
			imaqMeasureParticle(image, 0, false, IMAQ_MT_CENTER_OF_MASS_X,
					&frameCenterX);
		} else {
			// TODO: make sure that in pid commands you stop if it's 0
			frameCenterX = NAN;
		}
	} else {
		frameCenterX = NAN;
	}

	LCameraServer::GetInstance()->SetImage(image);
}

void VisionSubsystem::SetPIDSourceType(PIDSourceType pidSource) {
	// do nothing
}

PIDSourceType VisionSubsystem::GetPIDSourceType() const {
	return PIDSourceType::kDisplacement;
}

double VisionSubsystem::PIDGet() {
	return frameCenterX;
}
