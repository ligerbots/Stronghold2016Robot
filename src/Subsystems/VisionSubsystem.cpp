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
		mp_displayFrame(NULL),
		m_frameCenterX(0),
		m_frameCenterY(0),
		m_distance(0),
		m_angle(0),
		m_frameWidth(640.0), 		// guess
		m_numParticles(0),
		m_processingThread(&VisionSubsystem::visionProcessingThread,this),
		m_visionBusy(false),
		m_visionRequested(true), // run one vision frame on startup
		m_lastVisionTick(0),
		m_activeCamera(0),
		m_isScalingDown("VisionScaleDown"),
		m_pM(NULL)
{
	ledRingSpike.reset(new Relay(RobotMap::RELAY_LED_RING_SPIKE));
	enableVision = true;	// default on
	m_firstFrame = true;

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
	if (m_activeCamera != 0) {
		LCameraServer::GetInstance()->SetImage(image);
	} else {
		if (mp_processingFrame != NULL && showVision.get()) {
			LCameraServer::GetInstance()->SetImage(mp_processingFrame);
		} else {
			if(mp_displayFrame == NULL){
				mp_displayFrame = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
			}

			if(m_isScalingDown.get()){
				imaqScale(mp_displayFrame, image, 2, 2, IMAQ_SCALE_LARGER, IMAQ_NO_RECT);
			} else {
				imaqDuplicate(mp_displayFrame, image);
			}

			if (!isVisionCalculationDirty()) {
				// If the robot hasn't shifted more than 1.5 degrees off the orientation
				// it had when we last took a vision position, or more than 1 inch in position,
				// then display the target markup
				markTarget(mp_displayFrame);
			}
			LCameraServer::GetInstance()->SetImage(mp_displayFrame);
		}
	}

	if (m_visionRequested) {
		// If we just asked camera zero to get a frame, don't do it again here
		if (m_activeCamera != 0) Camera::GetCamera(0)->GetFrame();
		mp_currentFrame = Camera::GetCamera(0)->GetStoredFrame();
		int width, height;
		imaqGetImageSize(mp_currentFrame, &width, &height);
		if(m_isScalingDown.get()){
			m_frameWidth = ((double) width) * 2.0;
		} else {
			m_frameWidth = (double) width;
		}
		// We don't do a SetImage here -- that's done in the Vision Processing thread

		// If it's been more than eight vision ticks since we last processed a frame, do one now
		if ((Robot::ticks > m_lastVisionTick + 8) && !m_visionBusy) {
			if (mp_processingFrame == NULL) {
				// First time: create our processing frame
				printf("VisionSubsystem: Creating second Image*\n");
				mp_processingFrame = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
			}
			// duplicate the current frame for image processing
			if(m_isScalingDown.get()){
//				int processingFrameWidth, processingFrameHeight;
//				imaqGetImageSize(mp_processingFrame, &processingFrameWidth, &processingFrameHeight);
//				if(processingFrameWidth == 640){
//					imaqDispose(mp_processingFrame);
//					mp_processingFrame = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
//				}
//				// scale 2x down for a potential 4x processing speed increase
//				imaqResample(mp_processingFrame, mp_currentFrame, width/2, height/2,
//						InterpolationMethod::IMAQ_BILINEAR, imaqMakeRect(0, 0, height, width));
//
//				imaqGetImageSize(mp_processingFrame, &processingFrameWidth, &processingFrameHeight);
//				printf("Width: %d, height %d\n", processingFrameWidth, processingFrameHeight);
//
//				printf("Scaling image\n");
				int processingFrameWidth, processingFrameHeight;
								imaqGetImageSize(mp_processingFrame, &processingFrameWidth, &processingFrameHeight);
								if(processingFrameWidth != 640){
									imaqDispose(mp_processingFrame);
									mp_processingFrame = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
								}
								imaqDuplicate(mp_processingFrame, mp_currentFrame);
			} else {
				int processingFrameWidth, processingFrameHeight;
				imaqGetImageSize(mp_processingFrame, &processingFrameWidth, &processingFrameHeight);
				if(processingFrameWidth != 640){
					imaqDispose(mp_processingFrame);
					mp_processingFrame = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
				}
				imaqDuplicate(mp_processingFrame, mp_currentFrame);
			}
			m_visionBusy = true;
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
			if(err == 0){
				err = imaqGetLastError();
				printf("Error: %d\n", imaqGetLastError());
			}
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
			printf("Vision: Finished a vision frame\n");
			m_firstFrame = false;
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

			// assign to temporary variables first so that we don't
			// accidentally get unscaled center coordinates because
			// of threading
			// centerX and centerY fields are only set once per vision frame
			double frameCenterX = (feretStartX + feretEndX) / 2;
			double frameCenterY = (feretStartY + feretEndY) / 2;

			if(m_isScalingDown.get()){
				// scale coordinates back up to a 640x360 frame
				m_frameCenterX = frameCenterX * 2;
				m_frameCenterY = frameCenterY * 2;
			} else {
				m_frameCenterX = frameCenterX;
				m_frameCenterY = frameCenterY;
			}

			calculateDistanceAndAngle(m_frameCenterX, m_frameCenterY, &m_distance, &m_angle);
		}
	}
}

void VisionSubsystem::markTarget(Image *image) {
	if (paintTarget.get() && image!=NULL && m_pM!=NULL) {
		double feretStartX = m_pM[MFDSX];
		double feretStartY = m_pM[MFDSY];
		double feretEndX = m_pM[MFDEX];
		double feretEndY = m_pM[MFDEY];
		if(m_isScalingDown.get()){
			// scale coordinates back up to a 640x360 frame
			feretStartX *= 2;
			feretStartY *= 2;
			feretEndX *= 2;
			feretEndY *= 2;
		}
		// Mutex below is commented out because we're now painting the target on the main thread
		// std::lock_guard<std::mutex> lock(m_frameMutex);
		// Send the image to the dashboard with a target indicator
		int width, height;
		imaqGetImageSize(image, &width, &height);
		double setpoint = getCorrectedFrameCenter(m_distance);
		if (m_numParticles != 0) {
			// If the target is centered in our field of view, paint it green; else red
			double Xerror = fabs(setpoint * width - m_frameCenterX);
//			printf("%f\n", Xerror);
			// Centered means no more than 1.5% off to either side
			double color = Xerror < 3.2 ? GREEN : RED;

			if(false){
				// this code attempts to draw an circle, but ...
				// draw a 6-pixel circle in red
				imaqDrawShapeOnImage(image, image,
						{ (int) m_frameCenterY - 3, (int) m_frameCenterX - 3, 6, 6}, IMAQ_PAINT_VALUE, IMAQ_SHAPE_OVAL, color);
			}

				imaqDrawLineOnImage(image, image, DrawMode::IMAQ_DRAW_VALUE,
						{ (int) m_frameCenterX - 5, (int) m_frameCenterY },
						{ (int) m_frameCenterX + 5, (int) m_frameCenterY },
						CYAN);
				imaqDrawLineOnImage(image, image, DrawMode::IMAQ_DRAW_VALUE,
						{ (int) m_frameCenterX, (int) m_frameCenterY - 5 },
						{ (int) m_frameCenterX, (int) m_frameCenterY + 5 },
						CYAN);
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

double VisionSubsystem::getCorrectedFrameCenter(double distInches) {
	int width = m_frameWidth;

	if (width == 0) return 0; // no frame captured yet
	int center = width/2;
	double fCenter = (double) center;

	// no target, no correction
	if (m_numParticles>0) {
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

double VisionSubsystem::getTargetCenterX() {
	return m_frameCenterX;
}

double VisionSubsystem::getTargetCenterY() {
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
//	double centerOfMassY = getTargetCenterY();
//	return 2.333 * pow(1.0052, centerOfMassY);

	// use NewVision calculations
	return m_distance;
}

double VisionSubsystem::getFlapsFractionForDistance(double distance) {
	distance = distance / 12; // distances from the lookup table and regression are now in inches, so convert to feet
	// which is what the flaps lookup table needs

	// there's lots of complicated physics involved during the shot
	// no regression fits test data well, so this does linear interpolation
	// between lookup table values
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

	SmartDashboard::PutNumber("NewVision Target Distance", m_distance);
	SmartDashboard::PutNumber("NewVision Target Angle", m_angle);

	double angle_regBased;
	double distance_regBased;
	calculateDistanceAndAngle_FromRegression(m_frameCenterX, m_frameCenterY, &distance_regBased, &angle_regBased);
	SmartDashboard::PutNumber("Max's NewVision Target Distance", distance_regBased);
	SmartDashboard::PutNumber("Max's NewVision Target Angle", angle_regBased);
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
//	double centerToFraction = getCorrectedFrameCenter(m_distance);
//	double angle2 = atan(centerToFraction * tan_half_horizontal_field_of_view / 0.5) * 180 / M_PI;
	double angle1 = m_angle;
//	double angle = angle1 - angle2;
//	printf("---> targetX = %5.2f, fraction1 = %5.2f, angle1 = %5.2f angle2 = %5.2f\n", m_frameCenterX, centerToFraction, angle1, angle2);
//	printf("---> Angle to target relative %5.2f\n", angle);
	SmartDashboard::PutNumber("AngleToTarget", angle1);
	return angle1;
}

void VisionSubsystem::calculateDistanceAndAngle(double xpos, double ypos, double* distance, double* angle){
	calculateDistanceAndAngle_FromRegression(xpos, ypos, distance, angle);

	return; // don't use lookup table anymore

	FieldInfo::VisionDataPoint closestPoint;
	double closestPointMeasure = DBL_MAX;
	for(size_t i = 0; i < sizeof(Robot::instance->fieldInfo.visionData) / sizeof(FieldInfo::VisionDataPoint); i++){
		FieldInfo::VisionDataPoint point = Robot::instance->fieldInfo.visionData[i];
		double measure = (point.xpos - xpos) * (point.xpos - xpos) + (point.ypos - ypos) * (point.ypos - ypos);
		if(measure < closestPointMeasure){
			closestPoint = point;
			closestPointMeasure = measure;
		}
	}
	if(closestPointMeasure == DBL_MAX){
		printf("Vision: Can't fit distance/angle data\n");
		// should never happen
		*distance = 0;
		*angle = 0;
		return;
	}

	// find point with next angle to interpolate with
	double nextAngle = (xpos > closestPoint.xpos) ? closestPoint.angle + 10 : closestPoint.angle - 10;
	FieldInfo::VisionDataPoint pointNextAngle;
	bool angleFound = false;
	for(size_t i = 0; i < sizeof(Robot::instance->fieldInfo.visionData) / sizeof(FieldInfo::VisionDataPoint); i++){
		FieldInfo::VisionDataPoint point = Robot::instance->fieldInfo.visionData[i];
		if(point.distance == closestPoint.distance && point.angle == nextAngle){
			pointNextAngle = point;
			angleFound = true;
			break;
		}
	}

	// find point with next distance to interpolate with
	double nextDistance = (ypos > closestPoint.ypos) ? closestPoint.distance + 12 : closestPoint.distance - 12;
	FieldInfo::VisionDataPoint pointNextDistance;
	bool distanceFound = false;
	for(size_t i = 0; i < sizeof(Robot::instance->fieldInfo.visionData) / sizeof(FieldInfo::VisionDataPoint); i++){
		FieldInfo::VisionDataPoint point = Robot::instance->fieldInfo.visionData[i];
		if(point.distance == nextDistance && point.angle == closestPoint.angle){
			pointNextDistance = point;
			distanceFound = true;
			break;
		}
	}

	if(angleFound){
		*angle = closestPoint.angle + (xpos - closestPoint.xpos) * (pointNextAngle.angle - closestPoint.angle) / (pointNextAngle.xpos - closestPoint.xpos);
	} else {
		*angle = closestPoint.angle;
	}
	if(distanceFound){
		*distance = closestPoint.distance + (ypos - closestPoint.ypos) * (pointNextDistance.distance - closestPoint.distance) / (pointNextDistance.ypos - closestPoint.ypos);
	} else {
		*distance = closestPoint.distance;
	}
}

void VisionSubsystem::calculateDistanceAndAngle_FromRegression(double xpos, double ypos, double* distance, double* angle){
	*distance =
			regcoef_d1 * xpos +
			regcoef_d2 * xpos * xpos +
			regcoef_d3 * ypos +
			regcoef_d4 * ypos * ypos +
			regcoef_d5 * xpos * ypos +
			regcoef_d6;
	*angle =
			regcoef_a1 * xpos +
			regcoef_a2 * xpos * xpos +
			regcoef_a3 * ypos +
			regcoef_a4 * ypos * ypos +
			regcoef_a5 * xpos * ypos +
			regcoef_a6;
}

bool VisionSubsystem::isVisionCalculationDirty(){
	if(m_firstFrame || m_numParticles == 0){
		return true;
	}
	// checks for validity of vision calculation by comparing current robot position and orientation
	// to what it was during the calculation
	DriveSubsystem::Position pos = CommandBase::driveSubsystem->GetPosition();
	return fabs(pos.Angle - m_robotPos.Angle) > 0.5
			|| fabs(pos.X - m_robotPos.X) > 1 || fabs(pos.Y - m_robotPos.Y) > 1;
}

bool VisionSubsystem::isVisionBusy(){
	// for AcquireTarget with waitForVision=true
	// on the first tick, requested = true, busy = false
	// once updateVision runs (in 1 or 2 more ticks), requested = false, busy = true
	// once thread actually finishes processing, requested = false, busy = false
	return m_visionRequested || m_visionBusy;
}
