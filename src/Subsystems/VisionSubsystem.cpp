#include <Stronghold2016Robot.h>

VisionSubsystem::VisionSubsystem() :
		Subsystem("VisionSubsystem"),
			exposure("Exposure"),
			showVision("ShowVision"),
			paintTarget("PaintTarget"),
			color("DrawColor"),
			mp_currentFrame(NULL),
			mp_processingFrame(NULL),
			m_frameCenterX(0),
			m_frameCenterY(0),
			m_numParticles(0),
			m_processingThread(&VisionSubsystem::visionProcessingThread, this)
{
	activeCamera = 0;
	ledRingSpike.reset(new Relay(RobotMap::RELAY_LED_RING_SPIKE));
}

void VisionSubsystem::InitDefaultCommand() {
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
//		std::lock_guard<std::mutex> lock(m_frameMutex);
		Camera::Feed(ticks);
		image = Camera::GetCamera(activeCamera)->GetStoredFrame();
		mp_currentFrame = Camera::GetCamera(0)->GetStoredFrame();
	}

	if (!paintTarget.get() && !showVision.get() && image != NULL) {
		LCameraServer::GetInstance()->SetImage(image);
	}
}

void VisionSubsystem::toggleCameraFeed() {
	if (Camera::GetNumberOfCameras() > 1) {
		activeCamera = 1 - activeCamera; // 1 -> 0; 0 -> 1
	} else {
		activeCamera = 0;
	}
}

void VisionSubsystem::visionProcessingThread() {
	printf("VisionSubsystem: Processing thread start\n");

	timeval startTime;
	timeval endTime;
	while (true) {
		if (mp_currentFrame == NULL) {
			// wait for first frame
			usleep(34000); // 34 ms
			continue;
		}
		if (mp_processingFrame == NULL) {
			// create our processing frame
			printf("VisionSubsystem: Creating second Image*\n");
			mp_processingFrame = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
		}

		gettimeofday(&startTime, 0);

		{
//			std::lock_guard<std::mutex> lock(m_frameMutex);
			imaqDuplicate(mp_processingFrame, mp_currentFrame);
		}

		int err = IVA_ProcessImage(mp_processingFrame); // run vision script
		SmartDashboard::PutNumber("Vision/imaq_err", err);

		bool needsConnection = true;
		imaqCountParticles(mp_processingFrame, needsConnection, &m_numParticles);
		if (m_numParticles != 0) {
			imaqMeasureParticle(mp_processingFrame, 0, false,
					IMAQ_MT_CENTER_OF_MASS_X, &m_frameCenterX);
			imaqMeasureParticle(mp_processingFrame, 0, false,
					IMAQ_MT_CENTER_OF_MASS_Y, &m_frameCenterY);
		} else {
			// TODO: make sure that in pid commands you stop if it's NAN
//			frameCenterX = NAN;
		}

		if (paintTarget.get()) {
			// Send the image to the dashboard with a target indicator
			int width, height;
			imaqGetImageSize(mp_processingFrame, &width, &height);
			if (m_numParticles != 0) {
				int x = (int) m_frameCenterX;
				int y = (int) m_frameCenterY;
//				int centerX = width/2;
//				bool centered = (centerX < x+5) && (centerX > y -5);
				int top, left, rectheight, rectwidth;
				top = y + 5;
				left = x + 5;
				rectheight = 10;
				rectwidth = 10;

//				RGBValue rgbColor = centered ? IMAQ_RGB_GREEN : IMAQ_RGB_RED;
//				RGBValue *pColor = &rgbColor;


				imaqDrawShapeOnImage(mp_currentFrame, mp_currentFrame, {top, left, rectheight, rectwidth}, IMAQ_DRAW_VALUE, IMAQ_SHAPE_OVAL, color.get());
//				imaqOverlayOval(mp_currentFrame, {top, left, rectheight, rectwidth}, pColor, IMAQ_DRAW_VALUE, NULL);
			}
			imaqDrawLineOnImage(mp_currentFrame, mp_currentFrame, DrawMode::IMAQ_DRAW_VALUE,
										{width/2, 0}, {width/2, height}, color.get());
			LCameraServer::GetInstance()->SetImage(mp_currentFrame);
		} else if (showVision.get()) {
			LCameraServer::GetInstance()->SetImage(mp_processingFrame);
		}

		gettimeofday(&endTime, 0);

		__suseconds_t diff = endTime.tv_usec - startTime.tv_usec;
		if (diff > -50000)
			SmartDashboard::PutNumber("Vision/processingTime", diff);

		usleep(33000);
	}
}

double VisionSubsystem::getFrameCenter() {
	if (Camera::GetNumberOfCameras() < 1)
		return 0;
	else {
		int width = Camera::GetCamera(0)->GetWidth();
		if (width == 0)
			return 0; // no frame captured yet
		else
			return width / 2.0;
	}
}

void VisionSubsystem::sendValuesToSmartDashboard() {
	if (ledRingSpike->GetError().GetCode() != 0) {
		SmartDashboard::PutString("LED", "Not Present");
	} else {
		Relay::Value val = ledRingSpike->Get();
		if (val == Relay::kOff) {
			SmartDashboard::PutString("LED", "Off");
		} else if (val == Relay::kForward) {
			SmartDashboard::PutString("LED", "Forward");
		} else if (val == Relay::kReverse) {
			SmartDashboard::PutString("LED", "Reverse");
		}
	}

	SmartDashboard::PutNumber("CamerasCount", Camera::GetNumberOfCameras());
	SmartDashboard::PutBoolean("CamerasEnabled", Camera::IsEnabled());
	SmartDashboard::PutBoolean("CamerasOpen", Camera::IsOpen());
	SmartDashboard::PutNumber("TargetsDetected", m_numParticles);
	SmartDashboard::PutNumber("Target X Pos", m_frameCenterX);
	SmartDashboard::PutNumber("Target Y Pos", m_frameCenterY);
	SmartDashboard::PutNumber("TargetsDetected", m_numParticles);
}

void VisionSubsystem::SetPIDSourceType(PIDSourceType pidSource) {
	// do nothing
}

PIDSourceType VisionSubsystem::GetPIDSourceType() const {
	return PIDSourceType::kDisplacement;
}

double VisionSubsystem::PIDGet() {
	return m_frameCenterX / (getFrameCenter() * 2);
}

