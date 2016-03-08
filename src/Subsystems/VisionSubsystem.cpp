#include <Stronghold2016Robot.h>

double VisionSubsystem::angles[] = { -1, -1, -1, -1, .6, .5, .49, .48, .47, .455, .454, .453, .452 };

VisionSubsystem::VisionSubsystem() :
		Subsystem("VisionSubsystem"),
		exposure("Exposure"),
		showVision("ShowVision"),
		paintTarget("PaintTarget"),
		enableVision("EnableVision"),
		color("DrawColor"),
		mp_currentFrame(NULL),
		mp_processingFrame(NULL),
		m_frameCenterX(0),
		m_frameCenterY(0),
		m_numParticles(0),
		m_processingThread(&VisionSubsystem::visionProcessingThread,this),
		m_activeCamera(0),
		m_pM(NULL)
{
	ledRingSpike.reset(new Relay(RobotMap::RELAY_LED_RING_SPIKE));
	enableVision = true;	// default on

	mT[COMX] = IMAQ_MT_CENTER_OF_MASS_X;
	mT[COMY] = IMAQ_MT_CENTER_OF_MASS_Y;
	mT[CHA] = IMAQ_MT_CONVEX_HULL_AREA;
	mT[AREA] = IMAQ_MT_AREA;
	mT[BRW] = IMAQ_MT_BOUNDING_RECT_WIDTH;
	mT[BRH] = IMAQ_MT_BOUNDING_RECT_HEIGHT;
	mT[AVSL] = IMAQ_MT_AVERAGE_VERT_SEGMENT_LENGTH;
	mT[AHSL] = IMAQ_MT_AVERAGE_HORIZ_SEGMENT_LENGTH;
	mT[MFD] = IMAQ_MT_MAX_FERET_DIAMETER;
	mT[ERLS] = IMAQ_MT_EQUIVALENT_RECT_LONG_SIDE;
	mT[ERSS] = IMAQ_MT_EQUIVALENT_RECT_SHORT_SIDE;
	mT[ERSSF] = IMAQ_MT_EQUIVALENT_RECT_SHORT_SIDE_FERET;
	mT[MFDO] = IMAQ_MT_MAX_FERET_DIAMETER_ORIENTATION;
	mT[MFDSX] = IMAQ_MT_MAX_FERET_DIAMETER_START_X;
	mT[MFDSY] = IMAQ_MT_MAX_FERET_DIAMETER_START_Y;
	mT[MFDEX] = IMAQ_MT_MAX_FERET_DIAMETER_END_X;
	mT[MFDEY] = IMAQ_MT_MAX_FERET_DIAMETER_END_Y;
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
		// Feed ticks asks for a new frame from the currently selected camera
		Camera::Feed(ticks);

		if (enableVision.get()) {
			// If the active camera wasn't the Vision camera, then also get a new
			// frame from the vision camera
			if (m_activeCamera != 0) Camera::GetCamera(0)->GetFrame();
			mp_currentFrame = Camera::GetCamera(0)->GetStoredFrame();
			// we don't display the vision camera frame here -- that's done in the Vision thread
		}
		else {
			image = Camera::GetCamera(m_activeCamera)->GetStoredFrame();
			LCameraServer::GetInstance()->SetImage(image);
		}
	}
}

void VisionSubsystem::toggleCameraFeed() {
	m_activeCamera++;
	if (m_activeCamera >= Camera::GetNumberOfCameras()) {
		m_activeCamera = 0;
	}
	Camera::SwitchCamera(m_activeCamera);
	printf("Active camera: %d\n", m_activeCamera);
}

void VisionSubsystem::setCameraFeed(int whichCamera){
	m_activeCamera = whichCamera;
	while(m_activeCamera >= Camera::GetNumberOfCameras()){
		m_activeCamera--;
	}
	Camera::SwitchCamera(m_activeCamera);
	printf("Active camera: %d\n", m_activeCamera);
}

void VisionSubsystem::visionProcessingThread() {
	printf("VisionSubsystem: Processing thread start\n");

	timeval startTime;
	timeval endTime;
	int startTicks = Robot::ticks;

	gettimeofday(&startTime, 0);

	while (true) {
		if (enableVision.get()) {
			if (mp_currentFrame == NULL) {
				// wait for first frame
				usleep(34000); 	// 34 ms - about a tick and half
				continue;
			}

			if (mp_processingFrame == NULL) {
				// First time: create our processing frame
				printf("VisionSubsystem: Creating second Image*\n");
				mp_processingFrame = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
			}

			imaqDuplicate(mp_processingFrame, mp_currentFrame);

			int err = IVA_ProcessImage(mp_processingFrame); // run vision script
			SmartDashboard::PutNumber("Vision/imaq_err", err);

			// compute the distance, angle, etc. and mark target on currentFrame
			measureAndMark(mp_currentFrame, mp_processingFrame);

			// Display the marked frame, or the processing frame
			if (m_activeCamera == 0)
				LCameraServer::GetInstance()->SetImage(showVision.get() ? mp_processingFrame : mp_currentFrame);
		}
		else {
			// if we didn't process any images, display something
			// this probably only gets executed the first time
			//LCameraServer::GetInstance()->SetImage(mp_currentFrame);
		}

		gettimeofday(&endTime, 0);
		__suseconds_t diff = endTime.tv_usec - startTime.tv_usec;
		if (diff > -50000) {
			SmartDashboard::PutNumber("Vision/processingTime", diff);
		}
		int endTicks = Robot::ticks;
		double framesPerSec = 50.0/(endTicks - startTicks);
		SmartDashboard::PutNumber("Vision/FPS", framesPerSec);
		usleep(33000);
	}
}


// Measure particles and mark target
// image is the image to process
// mark is the image we mark the target on
void VisionSubsystem::measureAndMark(Image *mark, Image *image)
{
	// TODO: refactor into a single imaqMeasureParticles(...) call
	// also use largest particle only, and check (convex hull area)/(particle area)
	// to make sure it's about 2.2
	imaqCountParticles(image, true, &m_numParticles);
	if (m_numParticles != 0) {
		MeasureParticlesReport *mprArray =
				imaqMeasureParticles(image, IMAQ_CALIBRATION_MODE_PIXEL, mT,MAXVAL);
		if (NULL == mprArray) {
			int imaqError = imaqGetLastError();
			char *msg = imaqGetErrorText(imaqError);
			printf("imaqMeasureParticles failed code=%d, msg=%s\n", imaqError, msg);
		}
		else {
			// Find the particle with the largest area
			double partArea = 0.0;
			int largest = 0;
			for (int i = 0; i != m_numParticles; i++) {
				double *pixelMeasurements = mprArray->pixelMeasurements[i];
				if (pixelMeasurements[AREA] > partArea) {
					partArea = pixelMeasurements[AREA];
					largest = i;
				}
			}
			m_pM = mprArray->pixelMeasurements[largest];

			m_frameCenterX = m_pM[COMX];
			m_frameCenterY = m_pM[COMY];

			//double areaConvexHull = m_pM[CHA];
			//double areaParticle = m_pM[AREA];
			//double widthBoundingBox = pM[BRW];
			//double heightBoundingBox = pM[BRH];
			//double feret = pM[MFDO];
			double feretStartX = m_pM[MFDSX];
			double feretStartY = m_pM[MFDSY];
			double feretEndX = m_pM[MFDEX];
			double feretEndY = m_pM[MFDEY];

			m_frameCenterX = (feretStartX + feretEndX) / 2;
			m_frameCenterY = (feretStartY + feretEndY) / 2;

			if (paintTarget.get()) {
				// Send the image to the dashboard with a target indicator
				int width, height;
				imaqGetImageSize(mark, &width, &height);
				if (m_numParticles != 0) {
					// draw a 6-pixel circle in red
					imaqDrawShapeOnImage(mark, mark,
							{ (int) m_frameCenterY - 3, (int) m_frameCenterX - 3, 6, 6}, IMAQ_PAINT_VALUE, IMAQ_SHAPE_OVAL, 255.0);
					if (false) {
						// this code attempts to draw an X, but ...
						imaqDrawLineOnImage(mark, mark, DrawMode::IMAQ_DRAW_VALUE,
								{ (int) m_frameCenterX - 5, (int) m_frameCenterY },
								{ (int) m_frameCenterX + 5, (int) m_frameCenterY },
								200.0);
						imaqDrawLineOnImage(mark, mark, DrawMode::IMAQ_DRAW_VALUE,
								{ (int) m_frameCenterX, (int) m_frameCenterY - 5 },
								{ (int) m_frameCenterX, (int) m_frameCenterY + 5 },
							200.0);
					}
					// Draw the whole feret diagonal
					imaqDrawLineOnImage(mark, mark, DrawMode::IMAQ_DRAW_VALUE,
							{(int) feretStartX, (int) feretStartY},
							{(int) feretEndX,   (int) feretEndY },
							(256.0*256.0)*255.0);
				}

				double setpoint = getSetpoint();
				imaqDrawLineOnImage(mark, mark, DrawMode::IMAQ_DRAW_VALUE,
						{ (int) (setpoint * width), 0 },
						{ (int) (setpoint * width), height },
						256.0*255.0+255.0);
			}
		}
	}
}

double VisionSubsystem::getFrameCenter() {
	if (Camera::GetNumberOfCameras() < 1)
		return 0;
	else {
		int width = Camera::GetCamera(0)->GetWidth();
		if (width == 0)
			return 0; // no frame captured yet
		else {
			return width / 2.0;
		}
	}
}

double VisionSubsystem::getSetpoint(){
	if (Camera::GetNumberOfCameras() < 1)
			return 0;
	else {
		if (Camera::GetCamera(0)->GetWidth() == 0)
			return 0; // no frame captured yet
	}
	double distInches = getDistanceToTarget() * 12;
	double f = (getFrameCenter()) / 1.54857776;
	double dxPixels = camera_offset * f / distInches;
	return (getFrameCenter() - dxPixels) / Camera::GetCamera(0)->GetWidth();
}

double VisionSubsystem::getCenterOfMassX() {
	return m_frameCenterX;
}

double VisionSubsystem::getCenterOfMassY() {
	return m_frameCenterY;
}

double VisionSubsystem::getBoundingBoxWidth() {
	return m_pM[IMAQ_MT_BOUNDING_RECT_WIDTH];
}

double VisionSubsystem::getBoundingBoxHeight() {
	return m_pM[IMAQ_MT_BOUNDING_RECT_HEIGHT];
}

double VisionSubsystem::getDistanceToTarget() {
	// an exponential regression fits our data with r2=99.9%
	// TODO: recalculate with new data
	double centerOfMassY = getCenterOfMassY();
	return 2.333 * pow(1.0052, centerOfMassY);
}

double VisionSubsystem::getFlapsFractionForDistance(double distance) {
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

