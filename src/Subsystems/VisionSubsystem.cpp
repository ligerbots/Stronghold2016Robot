#include <Stronghold2016Robot.h>
#include <cfloat>

double VisionSubsystem::angles[] = { -1, -1, -1, -1, .65, .55, .54, .5, .48, .49, .5, .5, .5 };

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
		m_frameWidth(640.0), 		// guess
		m_numParticles(0),
		m_processingThread(&VisionSubsystem::visionProcessingThread,this),
		m_visionBusy(false),
		m_visionRequested(true),	// run one vision frame on startup
		m_lastVisionTick(0),
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
	m_robotPos = {0, 0, 0};
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
	if (m_activeCamera != 0) LCameraServer::GetInstance()->SetImage(image);
	else {
		if (mp_processingFrame != NULL && showVision.get())
			LCameraServer::GetInstance()->SetImage(mp_processingFrame);
		else {
			DriveSubsystem::Position pos = CommandBase::driveSubsystem->GetPosition();
			if (fabs(pos.Angle - m_robotPos.Angle < 1.5)) {
				// If the robot hasn't shifted more than 1.5 degrees off the orientation
				// it had when we last took a vision position, then display the target markup
				markTarget(image);
			}
			LCameraServer::GetInstance()->SetImage(image);
		}
	}

	if (m_visionRequested) {
		// If we just asked camera zero to get a frame, don't do it again here
		if (m_activeCamera != 0) Camera::GetCamera(0)->GetFrame();
		mp_currentFrame = Camera::GetCamera(0)->GetStoredFrame();
		int width, height;
		imaqGetImageSize(mp_currentFrame, &width, &height);
		m_frameWidth = (double) width;
		// We don't do a SetImage here -- that's done in the Vision Processing thread

		// If it's been more than eight vision ticks since we last processed a grame, do one now
		if ((Robot::ticks > m_lastVisionTick + 8) && !m_visionBusy) {
			if (mp_processingFrame == NULL) {
				// First time: create our processing frame
				printf("VisionSubsystem: Creating second Image*\n");
				mp_processingFrame = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
			}
			// duplicate the current frame for image processing
			imaqDuplicate(mp_processingFrame, mp_currentFrame);
			pthread_cond_signal(&m_threadCond);
			// if a vision request came in while we were still processing, cancel it
			m_visionRequested = false;
		}
	}
}

void VisionSubsystem::runVision() {
	m_visionRequested = true;
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
	printf("Vision state: %d\n", enabled);
	enableVision.set(enabled);
}

void VisionSubsystem::visionProcessingThread() {
	printf("VisionSubsystem: Processing thread start\n");
	int loopCounter = 0;
	clockid_t cid;
	pthread_getcpuclockid(pthread_self(), &cid);

	while (true) {
		// wait here forever until we get a signal
		pthread_cond_wait(&m_threadCond, &m_threadMutex);
		m_visionBusy = true;
		loopCounter++;
		int startTicks = Robot::ticks;
		double startTime = Robot::GetRTC();
		timespec startCPUTime, endCPUTime;
		clock_gettime(cid, &startCPUTime);

		// for now in the new scheme we don't allow Vision disable
		if (true && enableVision.get()) {
			if (mp_currentFrame == NULL) {
				// wait for first frame
				usleep(34000); 	// 34 ms - about a tick and half
				continue;
			}

			int err = IVA_ProcessImage(mp_processingFrame); // run vision script
			SmartDashboard::PutNumber("Vision/imaq_err", err);

			// compute the distance, angle, etc. and mark target on currentFrame
			measureTarget(mp_processingFrame);
			m_robotPos = CommandBase::driveSubsystem->GetPosition();

			// print out CPU statistics periodically, but not so often as to spam the console
			if (loopCounter%40 == 0) {
				double elapsedTime = Robot::GetRTC() - startTime;
				int elapsedTicks = Robot::ticks - startTicks;
				clock_gettime(cid, &endCPUTime);
				double startCPU = (double) startCPUTime.tv_sec + (double) startCPUTime.tv_nsec/1.0E9;
				double endCPU = (double) endCPUTime.tv_sec + (double) endCPUTime.tv_nsec/1.0E9;
				double elapsedCPUTime = endCPU - startCPU;
				printf("Vision frame done in %f seconds, %f CPU seconds, %d ticks\n",
						elapsedTime, elapsedCPUTime, elapsedTicks);
			}
		}
		else {
			// if we didn't process any images, display something
			// this probably only gets executed the first time
			//LCameraServer::GetInstance()->SetImage(mp_currentFrame);
			m_numParticles = 0;
		}
		m_visionBusy = false;
		m_visionRequested = false;
		m_lastVisionTick = Robot::ticks;
		// with pthread_cond_wait, we don't need Mutex anymore
		//usleep(33000);
	}
}

bool VisionSubsystem::isTargetVisible(){
	return m_numParticles > 0;
}

// Measure particles and mark target
// image is the image to process
// mark is the image we mark the target on
void VisionSubsystem::measureTarget(Image *image)
{
	//  use largest particle only, and check (convex hull area)/(particle area)
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
			double partArea = 0.0;
//			double minOrientationOffset = DBL_MAX;
			int particleToChoose = 0;
//			double highestY = DBL_MAX;

			for (int i = 0; i != m_numParticles; i++) {
				double *pixelMeasurements = mprArray->pixelMeasurements[i];
				if (pixelMeasurements[AREA] > partArea) {
					partArea = pixelMeasurements[AREA];
					particleToChoose = i;
				}

//				if(pixelMeasurements[ORIENT] < minOrientationOffset){
//					particleToChoose = i;
//					minOrientationOffset = pixelMeasurements[ORIENT];
//				}

//				if(pixelMeasurements[MFDO] < minOrientationOffset){
//					particleToChoose = i;
//					minOrientationOffset = pixelMeasurements[MFDO];
//				}

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
		}
	}
}

void VisionSubsystem::markTarget(Image *image) {
	if (paintTarget.get() && image!=NULL && m_pM!=NULL) {
		double feretStartX = m_pM[MFDSX];
		double feretStartY = m_pM[MFDSY];
		double feretEndX = m_pM[MFDEX];
		double feretEndY = m_pM[MFDEY];
		// Mutex below is commented out because we're now painting the target on the main thread
		// std::lock_guard<std::mutex> lock(m_frameMutex);
		// Send the image to the dashboard with a target indicator
		int width, height;
		imaqGetImageSize(image, &width, &height);
		double setpoint = getCorrectedFrameCenter();
		if (m_numParticles != 0) {
			// If the target is centered in our field of view, paint it green; else red
			double Xerror = fabs(setpoint * width - m_frameCenterX);
//					printf("%f\n", Xerror);
			// Centered means no more than 1.5% off to either side
			double color = Xerror < (width * 0.015) ? GREEN : RED;
			// draw a 6-pixel circle in red
			imaqDrawShapeOnImage(image, image,
					{ (int) m_frameCenterY - 3, (int) m_frameCenterX - 3, 6, 6}, IMAQ_PAINT_VALUE, IMAQ_SHAPE_OVAL, color);
			if (false) {
				// this code attempts to draw an X, but ...
				imaqDrawLineOnImage(image, image, DrawMode::IMAQ_DRAW_VALUE,
						{ (int) m_frameCenterX - 5, (int) m_frameCenterY },
						{ (int) m_frameCenterX + 5, (int) m_frameCenterY },
						CYAN);
				imaqDrawLineOnImage(image, image, DrawMode::IMAQ_DRAW_VALUE,
						{ (int) m_frameCenterX, (int) m_frameCenterY - 5 },
						{ (int) m_frameCenterX, (int) m_frameCenterY + 5 },
						CYAN);
			}
			// Draw the whole feret diagonal
			imaqDrawLineOnImage(image, image, DrawMode::IMAQ_DRAW_VALUE,
					{(int) feretStartX, (int) feretStartY},
					{(int) feretEndX,   (int) feretEndY },
					YELLOW);
		}

		imaqDrawLineOnImage(image, image, DrawMode::IMAQ_DRAW_VALUE,
				{ (int) (setpoint * width), 0 },
				{ (int) (setpoint * width), height },
				YELLOW);
	}
}

double VisionSubsystem::getCorrectedFrameCenter() {
	if (Camera::GetNumberOfCameras() < 1) return 0;
	int width = Camera::GetCamera(0)->GetWidth();

	if (width == 0) return 0; // no frame captured yet
	int center = width/2;
	double fCenter = (double) center;

	// no target, no correction
	if (m_numParticles>0) {
		double distInches = getDistanceToTarget() * 12;
	/*
		Charles' version ...
		double offsetAngle = atan2(camera_offset, distInches);
		double ratio = offsetAngle/horizontal_field_of_view;
		// ratio gives us the fraction of the field of view to adjust by.
		// now turn it into pixels
		double dxPixels = ratio * width;
		return (fCenter - dxPixels);
	*/
		// fov_diag = 90deg http://www.logitech.com/assets/47868/logitech-webcam-c930e-data-sheet.pdf
		// fov_horiz = 2 * atan2(16/2, sqrt(16*16+9*9)/2)
		// tan(fov_horiz/2) = .8716

		// charles's fov calculation - 78.442, tan(78.442/2) = 0.8162
		double f = (fCenter / tan_half_horizontal_field_of_view);
		double dxPixels = camera_offset * f / distInches;
		fCenter = (fCenter - dxPixels) / width;
	}
	return fCenter;
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

// m_frameCenterX is where Vision put the target
//
double VisionSubsystem::PIDGet() {
	return m_frameCenterX / m_frameWidth;
}

// returns the TargetAngle RELATIVE to the current robot angle
double VisionSubsystem::TargetAngle() {
	if (m_numParticles==0) return 0.0;
	double centerToFraction = getCorrectedFrameCenter();
	double frameCenterFraction = m_frameCenterX / m_frameWidth;
	double angle2 = atan(centerToFraction * tan_half_horizontal_field_of_view / 0.5) * 180 / M_PI;
	double angle1 = atan(frameCenterFraction * tan_half_horizontal_field_of_view / 0.5) * 180 / M_PI;
	double angle = angle1 - angle2;
	printf("---> targetX = %5.2f, fraction1 = %5.2f fraction2 = %5.2f, angle1 = %5.2f angle2 = %5.2f\n", m_frameCenterX, centerToFraction, frameCenterFraction, angle1, angle2);
	printf("---> Angle to target relative %5.2f\n", angle);
	SmartDashboard::PutNumber("AngleToTarget", angle);
	return angle;
}
