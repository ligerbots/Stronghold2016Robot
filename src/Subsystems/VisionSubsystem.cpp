#include <Stronghold2016Robot.h>
#include <cfloat>

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
	mT[ORIENT] = IMAQ_MT_ORIENTATION;
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

	if (enableVision.get()) setLedRingOn(true);		// just in case

	Camera::GetCamera(0)->SetExposure(exposure.get());
	// Get a frame from the current camera
	Camera::GetCamera(m_activeCamera)->GetFrame();
	Image *image = Camera::GetCamera(m_activeCamera)->GetStoredFrame();
	// if we're not running Vision, just display the frame from the current camera, or
	// if the alternate camera is current, display its frame, even if we're doing vision on camera 0
	if (!enableVision.get() || m_activeCamera != 0) LCameraServer::GetInstance()->SetImage(image);

	if (enableVision.get()) {
		// If we just asked camera zero to get a frame, don't do it again here
		if (m_activeCamera != 0) Camera::GetCamera(0)->GetFrame();
		mp_currentFrame = Camera::GetCamera(0)->GetStoredFrame();
		// We don't do a SetImage here -- that's done in the Vision Processing thread
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

void VisionSubsystem::setVisionEnabled(bool enabled){
	enableVision.set(enabled);
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
			printf("imaqMeasureParticles failed code=%d\n", imaqError);
		}
		else {
			// Find the particle with the largest area
//			double partArea = 0.0;
			double minOrientationOffset = DBL_MAX;
			int particleToChoose = 0;
//			double highestY = DBL_MAX;

			for (int i = 0; i != m_numParticles; i++) {
				double *pixelMeasurements = mprArray->pixelMeasurements[i];
//				if (pixelMeasurements[AREA] > partArea) {
//					partArea = pixelMeasurements[AREA];
//					particleToChoose = i;
//				}

				// trying out yet another measurement of which particle is the best to go for
				// we probably want the particle with its bottom side closest to horizontal
				// IMAQ measures orientation as the angle of the lowest moment of inertia line
				// to the horizontal
				// for the target, the lowest moment should be along the bottom side
				// IMAQ maps orientation to (0, 180) for you so we just need the smallest
				// orientation
				// another option would be to do something with max feret angle
				if(pixelMeasurements[ORIENT] < minOrientationOffset){
					particleToChoose = i;
					minOrientationOffset = pixelMeasurements[ORIENT];
				}

//				double feretStartY = pixelMeasurements[MFDSY];
//				double feretEndY = pixelMeasurements[MFDEY];
//
//				double thisParticleY = (feretStartY + feretEndY) / 2;
//				if (thisParticleY < highestY) {
//					highestY = thisParticleY;
//					particleToChoose = i;
//				}
			}
			m_pM = mprArray->pixelMeasurements[particleToChoose];

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
				double setpoint = getSetpoint();
				if (m_numParticles != 0) {
					// If the target is centered in our field of view, paint it green; else red
					double Xerror = fabs(setpoint * width - m_frameCenterX);
//					printf("%f\n", Xerror);
					// Centered means no more than 1.5% off to either side
					double color = Xerror < (width * 0.015) ? GREEN : RED;
					// draw a 6-pixel circle in red
					imaqDrawShapeOnImage(mark, mark,
							{ (int) m_frameCenterY - 3, (int) m_frameCenterX - 3, 6, 6}, IMAQ_PAINT_VALUE, IMAQ_SHAPE_OVAL, color);
					if (false) {
						// this code attempts to draw an X, but ...
						imaqDrawLineOnImage(mark, mark, DrawMode::IMAQ_DRAW_VALUE,
								{ (int) m_frameCenterX - 5, (int) m_frameCenterY },
								{ (int) m_frameCenterX + 5, (int) m_frameCenterY },
								CYAN);
						imaqDrawLineOnImage(mark, mark, DrawMode::IMAQ_DRAW_VALUE,
								{ (int) m_frameCenterX, (int) m_frameCenterY - 5 },
								{ (int) m_frameCenterX, (int) m_frameCenterY + 5 },
								CYAN);
					}
					// Draw the whole feret diagonal
					imaqDrawLineOnImage(mark, mark, DrawMode::IMAQ_DRAW_VALUE,
							{(int) feretStartX, (int) feretStartY},
							{(int) feretEndX,   (int) feretEndY },
							YELLOW);
				}

				imaqDrawLineOnImage(mark, mark, DrawMode::IMAQ_DRAW_VALUE,
						{ (int) (setpoint * width), 0 },
						{ (int) (setpoint * width), height },
						YELLOW);
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
	if (Camera::GetNumberOfCameras() < 1)return 0;
	int width = Camera::GetCamera(0)->GetWidth();

	if (width == 0) return 0; // no frame captured yet

	double distInches = getDistanceToTarget() * 12;
/*
	Charles' version ...
	double offsetAngle = atan2(camera_offset, distInches);
	double ratio = offsetAngle/horizontal_field_of_view;
	// ratio gives us the fraction of the field of view to adjust by.
	// now turn it into pixels
	double dxPixels = ratio * width;
	return (getFrameCenter() - dxPixels);
*/
	// fov_diag = 90deg http://www.logitech.com/assets/47868/logitech-webcam-c930e-data-sheet.pdf
	// fov_horiz = 2 * atan2(16/2, sqrt(16*16+9*9)/2)
	// tan(fov_horiz/2) = .8716

	// charles's fov calculation - 78.442
	double f = (getFrameCenter() / 0.8162);
	double dxPixels = camera_offset * f / distInches;
	return (getFrameCenter() - dxPixels) / width;
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
	SmartDashboard::PutNumber("TargetDistance", getDistanceToTarget());
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

