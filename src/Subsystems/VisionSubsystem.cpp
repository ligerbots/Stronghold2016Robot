#include <Stronghold2016Robot.h>

VisionSubsystem::VisionSubsystem() :
		Subsystem("VisionSubsystem"),
		exposure("Exposure"),
		showVision("ShowVision"),
		paintTarget("PaintTarget"),
		color("DrawColor"),
		enableVision("EnableVision"),
		boundingBoxWidth("VisionBoundingBoxWidth", false),
		boundingBoxHeight("VisionBoundingBoxHeight", false),
		convexHullSize("VisionCHSize", false),
		convexHullPerArea("VisionCHPerArea",false),
		feretDiameter("VisionFeretDiameter", false),
		mp_currentFrame(NULL),
		mp_processingFrame(NULL),
		m_frameCenterX(0),
		m_frameCenterY(0),
		m_numParticles(0),
		m_processingThread(&VisionSubsystem::visionProcessingThread,this),
		m_inVision(false)
{
	activeCamera = 0;
	ledRingSpike.reset(new Relay(RobotMap::RELAY_LED_RING_SPIKE));
	enableVision = true;	// default on
	color = 0;
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
		if (!enableVision) {
			Camera::Feed(ticks);
			image = Camera::GetCamera(activeCamera)->GetStoredFrame();
			mp_currentFrame = Camera::GetCamera(0)->GetStoredFrame();
		}
		else {
			bool inVision;
			{
				//std::lock_guard<std::mutex> lock(m_frameMutex);
				inVision = m_inVision;
			}
			if (!inVision) {
				Camera::Feed(ticks);
				image = Camera::GetCamera(activeCamera)->GetStoredFrame();
				mp_currentFrame = Camera::GetCamera(0)->GetStoredFrame();
			}
		}
	}

	if (image != NULL && (!enableVision.get() || (!paintTarget.get() && !showVision.get())) ) {
		LCameraServer::GetInstance()->SetImage(image);
	}
}

void VisionSubsystem::toggleCameraFeed() {
	activeCamera++;
	if (activeCamera >= Camera::GetNumberOfCameras()) {
		activeCamera = 0;
	}
	Camera::SwitchCamera(activeCamera);
	printf("Active camera: %d\n", activeCamera);
}

void VisionSubsystem::visionProcessingThread() {
	printf("VisionSubsystem: Processing thread start\n");

	timeval startTime;
	timeval endTime;

	gettimeofday(&startTime, 0);
	color.set(color.get() + 1000);

	while (true) {
		if (enableVision.get()) {
			{
				//std::lock_guard<std::mutex> lock(m_frameMutex);
				m_inVision = true;;
			}
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



			{
	//			std::lock_guard<std::mutex> lock(m_frameMutex);
				mp_processingFrame = mp_currentFrame;
				//imaqDuplicate(mp_processingFrame, mp_currentFrame);
			}

			int err = IVA_ProcessImage(mp_processingFrame); // run vision script

			SmartDashboard::PutNumber("Vision/imaq_err", err);

			// TODO: refactor into a single imaqMeasureParticles(...) call
			// also use largest particle only, and check (convex hull area)/(particle area)
			// to make sure it's about 2.2
			bool needsConnection = true;

			imaqCountParticles(mp_processingFrame, needsConnection,
					&m_numParticles);
			if (m_numParticles != 0) {
				MeasureParticlesReport *mprArray = imaqMeasureParticles(
						mp_processingFrame, IMAQ_CALIBRATION_MODE_PIXEL, mT,
						MAXVAL);

				// Find the particle with the largest area
				double partArea = 0.0;
				int largest = 0;
				double *pM;
				for (int i = 0; i != m_numParticles; i++) {
					double *pixelMeasurements = mprArray->pixelMeasurements[i];
					if (pixelMeasurements[AREA] > partArea) {
						partArea = pixelMeasurements[AREA];
						largest = i;
					}
				}
				pM = mprArray->pixelMeasurements[largest];

				m_frameCenterX = pM[COMX];
				m_frameCenterY = pM[COMY];

				double areaConvexHull;
				double areaParticle;
				double widthBoundingBox;
				double heightBoundingBox;
				double feret;
				double feretStartX, feretStartY, feretEndX, feretEndY;

				imaqMeasureParticle(mp_processingFrame, 0, false,
						IMAQ_MT_CONVEX_HULL_AREA, &areaConvexHull);
				imaqMeasureParticle(mp_processingFrame, 0, false, IMAQ_MT_AREA,
						&areaParticle);
				//imaqMeasureParticle(mp_processingFrame, 0, false, IMAQ_MT_BOUNDING_RECT_WIDTH, &widthBoundingBox);
				//imaqMeasureParticle(mp_processingFrame, 0, false, IMAQ_MT_BOUNDING_RECT_HEIGHT, &heightBoundingBox);

				imaqMeasureParticle(mp_processingFrame, 0, false,
						IMAQ_MT_EQUIVALENT_RECT_LONG_SIDE, &widthBoundingBox);
				imaqMeasureParticle(mp_processingFrame, 0, false,
						IMAQ_MT_EQUIVALENT_RECT_SHORT_SIDE, &heightBoundingBox);

				imaqMeasureParticle(mp_processingFrame, 0, false,
						IMAQ_MT_MAX_FERET_DIAMETER, &feret);

				imaqMeasureParticle(mp_processingFrame, 0, false,
						IMAQ_MT_MAX_FERET_DIAMETER_START_X, &feretStartX);
				imaqMeasureParticle(mp_processingFrame, 0, false,
						IMAQ_MT_MAX_FERET_DIAMETER_START_Y, &feretStartY);
				imaqMeasureParticle(mp_processingFrame, 0, false,
						IMAQ_MT_MAX_FERET_DIAMETER_END_X, &feretEndX);
				imaqMeasureParticle(mp_processingFrame, 0, false,
						IMAQ_MT_MAX_FERET_DIAMETER_END_Y, &feretEndY);

				feretDiameter = feret;
				this->feretStartX = feretStartX;
				this->feretStartY = feretStartY;
				this->feretEndX = feretEndX;
				this->feretEndY = feretEndY;

				m_frameCenterX = (feretStartX + feretEndX) / 2;
				m_frameCenterY = (feretStartY + feretEndY) / 2;

				convexHullPerArea = areaConvexHull / areaParticle;
				convexHullSize = areaConvexHull;
				boundingBoxWidth = widthBoundingBox;
				boundingBoxHeight = heightBoundingBox;
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
	//				int top, left, rectheight, rectwidth;
	//				top = y + 5;
	//				left = x + 5;
	//				rectheight = 10;
	//				rectwidth = 10;

	//				RGBValue rgbColor = centered ? IMAQ_RGB_GREEN : IMAQ_RGB_RED;
	//				RGBValue *pColor = &rgbColor;

	//				imaqDrawShapeOnImage(mp_currentFrame, mp_currentFrame, { top,
	//						left, rectheight, rectwidth }, IMAQ_PAINT_VALUE,
	//						IMAQ_SHAPE_OVAL, color.get());
					imaqDrawLineOnImage(mp_currentFrame, mp_currentFrame,
							DrawMode::IMAQ_DRAW_VALUE, { (int) m_frameCenterX - 5,
									(int) m_frameCenterY },
							{ (int) m_frameCenterX + 5, (int) m_frameCenterY },
							color.get());
					imaqDrawLineOnImage(mp_currentFrame, mp_currentFrame,
							DrawMode::IMAQ_DRAW_VALUE,
							{ (int) m_frameCenterX, (int) m_frameCenterY - 5 },
							{ (int) m_frameCenterX, (int) m_frameCenterY + 5 },
							color.get());
	//				imaqOverlayOval(mp_currentFrame, {top, left, rectheight, rectwidth}, pColor, IMAQ_DRAW_VALUE, NULL);
					imaqDrawLineOnImage(mp_currentFrame, mp_currentFrame,
							DrawMode::IMAQ_DRAW_VALUE, { (int) feretStartX.get(),
									(int) feretStartY.get() }, {
									(int) feretEndX.get(), (int) feretEndY.get() },
							color.get());
				}
				imaqDrawLineOnImage(mp_currentFrame, mp_currentFrame,
						DrawMode::IMAQ_DRAW_VALUE, { width / 2, 0 }, { width / 2,
								height }, color.get());
				LCameraServer::GetInstance()->SetImage(mp_currentFrame);
			} else if (showVision.get()) {
				LCameraServer::GetInstance()->SetImage(mp_processingFrame);
			}


			gettimeofday(&endTime, 0);
			__suseconds_t diff = endTime.tv_usec - startTime.tv_usec;
			if (diff > -50000) {
				printf("Vision %ld\n", diff/1000);
				SmartDashboard::PutNumber("Vision/processingTime", diff);
			}
		}
		{
			//std::lock_guard<std::mutex> lock(m_frameMutex);
			m_inVision = false;
		}
		usleep(2000);
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

double VisionSubsystem::getCenterOfMassX() {
	return m_frameCenterX;
}

double VisionSubsystem::getCenterOfMassY() {
	return m_frameCenterY;
}

double VisionSubsystem::getBoundingBoxWidth() {
	return boundingBoxWidth.get();
}

double VisionSubsystem::getBoundingBoxHeight() {
	return boundingBoxHeight.get();
}

double VisionSubsystem::getDistanceToTarget() {
	// an exponential regression fits our data with r2=99.9%
	// TODO: recalculate with new data
	double centerOfMassY = getCenterOfMassY();
	return 2.333 * pow(1.0052, centerOfMassY);
}

double VisionSubsystem::getFlapsFractionForDistance(double distance) {
	double angles[] = { -1, -1, -1, -1, .6, .5, .49, .48, .47, .455, .454, .453, .452 };
	distance = fmax(fmin(distance, 9), 4);
	double low = angles[(int) floor(distance)];
	double high = angles[(int) ceil(distance)];
	double flapAngle = low + (high - low) * (distance - floor(distance));
	return flapAngle;
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
		SmartDashboard::PutBoolean("Vision/LedOn", val != Relay::kOff);
	}

	SmartDashboard::PutNumber("CamerasCount", Camera::GetNumberOfCameras());
	SmartDashboard::PutBoolean("CamerasEnabled", Camera::IsEnabled());
	SmartDashboard::PutBoolean("CamerasOpen", Camera::IsOpen());
	SmartDashboard::PutNumber("TargetsDetected", m_numParticles);
	SmartDashboard::PutNumber("Target X Pos", m_frameCenterX);
	SmartDashboard::PutNumber("Target Y Pos", m_frameCenterY);
	SmartDashboard::PutNumber("XPos", m_frameCenterX);
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

