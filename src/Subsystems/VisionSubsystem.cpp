#include <Stronghold2016Robot.h>

VisionSubsystem::VisionSubsystem() :
		Subsystem("VisionSubsystem"), exposure("VisionSubsystem_exposure"), showVision(
				"VisionSubsystem_showProcessing"), m_processingThread(
				&VisionSubsystem::visionProcessingThread, this), frameCenterX(
				0), frameCenterXParam("VisionSubsystem_frameCenterX", false) {
	mp_currentFrame = NULL;
	mp_processingFrame = NULL;
	ledRingSpike.reset(new Relay(RobotMap::LED_RING_SPIKE));
}

void VisionSubsystem::InitDefaultCommand() {
	// TODO: led toggle command
}

void VisionSubsystem::camerasOn() {
	printf("VisionSubsystem: camerasOn\n");
	Camera::EnumerateCameras();
	Camera::EnableCameras();
}

bool VisionSubsystem::isLedRingOn() {
	return ledRingSpike->Get() != Relay::kOff;
}

void VisionSubsystem::setLedRingOn(bool on) {
	if (on) {
		ledRingSpike->Set(Relay::kForward);
	} else {
		ledRingSpike->Set(Relay::kOff);
	}
}

void VisionSubsystem::updateVision(int ticks) {
	// run every second tick
	if (ticks % 2 == 0)
		return;

	if (Camera::GetNumberOfCameras() < 1)
		return;

	Camera::GetCamera(0)->SetExposure(exposure.get());

	Image* image = NULL;
	{
		std::lock_guard<std::mutex> lock(m_frameMutex);
		Camera::Feed(ticks);
		image = Camera::GetCamera(0)->GetStoredFrame();
		mp_processingFrame = image;
	}
	if (!showVision.get() && image != NULL)
		LCameraServer::GetInstance()->SetImage(image);
}

void VisionSubsystem::visionProcessingThread() {
	printf("VisionSubsystem: Processing thread start\n");
	while (true) {
		if (mp_currentFrame == NULL) {
			// wait for first frame
			usleep(34000); // 34 ms
			continue;
		}
		if (mp_processingFrame == NULL) {
			// create our processing frame
			mp_processingFrame = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
		}

		{
			std::lock_guard<std::mutex> lock(m_frameMutex);
			Rect rect;
			rect.top = 0;
			rect.left = 0;
			imaqGetImageSize(mp_currentFrame, &rect.width, &rect.height);
			Point pt;
			pt.x = 0;
			pt.y = 0;
			imaqCopyRect(mp_processingFrame, mp_currentFrame, rect, pt);
		}

		//IVA_ProcessImage (mp_processingFrame); // run vision script

//		int numParticles;
//		bool needsConnection = true;
//		imaqCountParticles(mp_processingFrame, needsConnection, &numParticles);
//		if (numParticles != 0) {
//			imaqMeasureParticle(mp_processingFrame, 0, false,
//					IMAQ_MT_CENTER_OF_MASS_X, &frameCenterX);
//		} else {
//			// TODO: make sure that in pid commands you stop if it's NAN
//			frameCenterX = NAN;
//		}
//	frameCenterXParam = frameCenterX;

		if (showVision.get()) {
			LCameraServer::GetInstance()->SetImage(mp_processingFrame);
		}
	}
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
