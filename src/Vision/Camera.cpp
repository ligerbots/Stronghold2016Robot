#include <Stronghold2016Robot.h>

uInt32 Camera::cameraCount = 0;
uInt32 Camera::currentCamera = 0;
IMAQdxCameraInformation Camera::camInfo[6];
Camera *Camera::cameras[6];
bool Camera::enabled;
bool Camera::overlay = false;
bool Camera::debugOutput;

/*
 Some selected camera modes (from Microsoft Lifecam Cinema or HD 3000 -- they're the same
 Video mode[19] is 424 x 240 YUY 2 30.00 fps = 19
 Video mode[20] is 424 x 240 YUY 2 20.00 fps = 20
 Video mode[21] is 424 x 240 YUY 2 15.00 fps = 21

 Video mode[48] is 640 x 480 JPEG 30.00 fps = 48
 Video mode[49] is 640 x 480 JPEG 20.00 fps = 49
 Video mode[50] is 640 x 480 JPEG 15.00 fps = 50

 Video mode[63] is 800 x 448 JPEG 30.00 fps = 63
 Video mode[64] is 800 x 448 JPEG 20.00 fps = 64
 Video mode[65] is 800 x 448 JPEG 15.00 fps = 65

 Video mode[68] is 640 x 360 JPEG 30.00 fps = 68
 Video mode[69] is 640 x 360 JPEG 20.00 fps = 69
 Video mode[70] is 640 x 360 JPEG 15.00 fps = 70

 Video mode[78] is 416 x 240 JPEG 30.00 fps = 78
 Video mode[79] is 416 x 240 JPEG 20.00 fps = 79
 Video mode[80] is 416 x 240 JPEG 15.00 fps = 80

 Video mode[83] is 352 x 288 JPEG 30.00 fps = 83
 Video mode[84] is 352 x 288 JPEG 20.00 fps = 84
 Video mode[85] is 352 x 288 JPEG 15.00 fps = 85

 Video mode[93] is 320 x 240 JPEG 30.00 fps = 93
 Video mode[94] is 320 x 240 JPEG 20.00 fps = 94
 Video mode[95] is 320 x 240 JPEG 15.00 fps = 95


 */
// i is the index of the camera we want to open
Camera::Camera(uInt32 i) :
		imaqError() {
	debugOutput = false;
	if (cameraCount == 0 && i > cameraCount) {
		printf(
				"Camera: attempt to open camera out of range (zero or too big).\n");
		return;
	}
	camera = i;
	firstTime = true;
	frame = NULL;
	session = ULONG_MAX;
}

Image* Camera::GetStoredFrame(){
	return frame;
}

void Camera::SetExposure(int percent) {
	static constexpr char const *ATTR_EX_MODE =
			"CameraAttributes::Exposure::Mode";
	static constexpr char const *ATTR_EX_VALUE =
			"CameraAttributes::Exposure::Value";
	static constexpr char const* MANUAL = "Manual";
	IMAQdxSetAttribute(session, ATTR_EX_MODE, IMAQdxValueTypeString, MANUAL);
	double minv = 0.0;
	double maxv = 0.0;
	IMAQdxGetAttributeMinimum(session, ATTR_EX_VALUE, IMAQdxValueTypeF64,
			&minv);
	IMAQdxGetAttributeMaximum(session, ATTR_EX_VALUE, IMAQdxValueTypeF64,
			&maxv);

	SmartDashboard::PutNumber("Exposure Min", minv);
	SmartDashboard::PutNumber("Exposure Max", maxv);

	double val = minv + ((maxv - minv) * ((double) percent / 100.0));
	IMAQdxSetAttribute(session, ATTR_EX_VALUE, IMAQdxValueTypeF64, val);
}

void Camera::DumpExposureSettings() {
	static constexpr char const *ATTR_EX_VALUE =
			"CameraAttributes::Exposure::Value";
	double minv = 0.0;
	double maxv = 0.0;
	IMAQdxGetAttributeMinimum(session, ATTR_EX_VALUE, IMAQdxValueTypeF64,
			&minv);
	IMAQdxGetAttributeMaximum(session, ATTR_EX_VALUE, IMAQdxValueTypeF64,
			&maxv);
	printf("Exposure min %4.2f max %4.2f\n", minv, maxv);
}

char *Camera::GetName() {
	return camInfo[currentCamera].InterfaceName;
}

char *Camera::GetModel() {
	return camInfo[currentCamera].ModelName;
}

void Camera::StartCameras() {
	for (uInt32 i = 0; i != cameraCount; i++)
		cameras[i]->Start();
}

void Camera::StopCameras() {
	for (uInt32 i = 0; i != cameraCount; i++)
		cameras[i]->Stop();
}

// static function for enumerating cameras
int Camera::EnumerateCameras() {
	uInt32 count = sizeof(camInfo) / sizeof(camInfo[0]);
	// last parameter is "connectedOnly". Why would we want to enumerate non-connected cameras?
	IMAQdxEnumerateCameras(camInfo, &count, true);
	cameraCount = count;
	printf("Cameras (%u):\n", (unsigned int) cameraCount);
	for (uInt32 i = 0; i != cameraCount; i++) {
		cameras[i] = new Camera(i);
		printf("  %s / %s / %s\n", camInfo[i].VendorName, camInfo[i].ModelName,
				camInfo[i].InterfaceName);
	}
	return cameraCount;
}

int Camera::GetNumberOfCameras() {
	return cameraCount;
}

Camera* Camera::GetCamera(int number) {
	return cameras[number];
}

uInt32 Camera::SwitchCamera() {
	if (++currentCamera >= cameraCount)
		currentCamera = 0;
	return currentCamera;
}

void Camera::EnableCameras() {
	StartCameras();
	enabled = true;
	// the most verbose debug output we only do the first time
	debugOutput = false;
}

void Camera::ToggleOverlay() {
	overlay = !overlay;
}

void Camera::DisableCameras() {
	StopCameras();
	enabled = false;
}

bool Camera::IsEnabled() {
	return enabled;
}

bool Camera::IsOpen() {
	// See if any camera is open
	bool open = false;
	for (uInt32 i = 0; i != cameraCount; i++)
		open |= (cameras[i]->session != ULONG_MAX);
	return open;
}

IMAQdxError Camera::Start() {
	// create an image
	printf("Opening camera %s\n", camInfo[camera].InterfaceName);
	imaqError = IMAQdxOpenCamera(camInfo[camera].InterfaceName,
			IMAQdxCameraControlModeController, &session);
	if (imaqError != IMAQdxErrorSuccess) {
		printf("IMAQdxOpenCamera error: %x\n", imaqError);
		session = ULONG_MAX;
		return imaqError;
	}
	// imaqError = Camera::SetMode();
	if (strstr(camInfo[camera].ModelName, "LifeCam") != nullptr)
		SetVideoMode(416, 240, 15, false);
	else if (strstr(camInfo[camera].ModelName, "Logitech Webcam") != nullptr)
		SetVideoMode(640, 360, 24, false);
	else {
		if (!SetVideoMode(800, 600, 10, false)) // Genuis wide angle camera, low frame rate
			SetVideoMode(320, 240, 15, false); // default to a widely supported mode
	}
	if (imaqError != IMAQdxErrorSuccess) {
		return imaqError;
	}
	imaqError = IMAQdxConfigureGrab(session);
	if (imaqError != IMAQdxErrorSuccess) {
		printf("IMAQdxOpenCamera error: %x\n", imaqError);
		return imaqError;
	}
	// start to acquire images
	IMAQdxStartAcquisition(session);
	if (imaqError != IMAQdxErrorSuccess) {
		printf("IMAQdxOpenCamera error: %x\n", imaqError);
		return imaqError;
	}

	frame = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
	printf("Camera Opened for %s #: %lu, frame=%x\n",
			camInfo[camera].InterfaceName, session, (unsigned int) frame);
	return imaqError;
}

IMAQdxError Camera::Stop() {
	// stop image acquisition
	imaqError = IMAQdxErrorSuccess;
	if (ULONG_MAX != session) {
		// Had trouble with imaqDrawTextOnImage
		//const DrawTextOptions options = {"Arial", 12, 0, 0, 0, 0, IMAQ_CENTER, IMAQ_INVERT};
		//int fontUsed;
		//imaqDrawTextOnImage(frame, frame, {120, 80}, "X", &options, &fontUsed);
		if (NULL != frame && camera == currentCamera) {
			// Would really like to make this semi-transparent, but not apparent how.
			int x, y;
			imaqGetImageSize(frame, &x, &y);
			//RGBValue color = IMAQ_RGB_YELLOW;
			//color.G = 240;
			//color.R = 240;
			//imaqOverlayOval(frame, { y/3, x/3, y/3, x/3 }, &color, IMAQ_DRAW_VALUE, NULL);
			//imaqMergeOverlay(NULL, frame, NULL, 0, NULL);
			// imaqDrawShapeOnImage(frame, frame, { y/3, x/3, y/3, x/3 }, DrawMode::IMAQ_DRAW_INVERT, ShapeMode::IMAQ_SHAPE_OVAL,0.0);
			LCameraServer::GetInstance()->SetImage(frame);
		}
		IMAQdxStopAcquisition(session);
		IMAQdxCloseCamera(session);
		printf("Camera %s closed on session %lu.\n",
				camInfo[camera].InterfaceName, session);
		session = ULONG_MAX;
	}
	return imaqError;
}

// Feed video frams from the currently selected camera.
// Pass in the robot tick count -- it's just wasteful to
// send a frame every tick if the camera rate is slower
void Camera::Feed(int ticks) {
	if (enabled && cameraCount > 0 && ticks % 2 == 0) {
		IMAQdxError imaqError = cameras[currentCamera]->GetFrame();
		if (cameras[currentCamera]->frame != NULL) {
			if (IMAQdxErrorSuccess == imaqError) {
				// TODO: take overlay code out into visionsubsystem
				overlay = false;
				if (overlay) {
					int x, y;
					Image *frame = cameras[currentCamera]->frame;
					imaqGetImageSize(frame, &x, &y);

					// Draw box around image. Note -- because the camera is pointing up a little, it's not a rectangle
					Point leftStart, leftEnd, rightStart, rightEnd;
					// It's just magic numbers here. See the "Feeder station image measurements.xlsx" spreadsheet
					leftStart.x = x * 0.387;
					leftStart.y = y * 0.258;
					leftEnd.x = x * 0.381;
					leftEnd.y = y * (0.258 + 0.584);
					imaqDrawLineOnImage(frame, frame, DrawMode::IMAQ_DRAW_VALUE,
							leftStart, leftEnd, 1.0);
					rightStart.x = x * (0.387 + 0.241);
					rightStart.y = y * 0.258;
					rightEnd.x = x * (0.381 + 0.249);
					rightEnd.y = y * (0.258 + 0.584);
					imaqDrawLineOnImage(frame, frame, DrawMode::IMAQ_DRAW_VALUE,
							rightStart, rightEnd, 1.0);
				}
			}
		}
	}
}

void Camera::DumpModes() {
	IMAQdxVideoMode videoModes[120];
	uInt32 count;
	uInt32 mode;
	IMAQdxEnumerateVideoModes(session, videoModes, &count, &mode);
	printf("There are %lu video modes, and the current mode is %lu\n", count,
			mode);
	for (uInt32 i = 0; i != count; i++) {
		printf("Video mode[%lu] is %s = %lu\n", i, videoModes[i].Name,
				videoModes[i].Value);
	}
}

bool Camera::SetVideoMode(unsigned int reqx, unsigned int reqy,
		unsigned int reqfps, bool use_Jpeg) {
	uInt32 count;
	uInt32 currentMode;

	if (debugOutput)
		printf("Set Video Mode, requesting %u x %u at %u fps %s.\n", reqx, reqy,
				reqfps, use_Jpeg ? "use JPEG" : "not JPEG");
	// most of this is copied from wpilibC++Devices/src/USBCamera.cpp
	IMAQdxEnumerateVideoModes(session, NULL, &count, &currentMode);	// first call to get the count of modes
	IMAQdxVideoMode modes[count];

	// then get the modes
	IMAQdxEnumerateVideoModes(session, modes, &count, &currentMode);
	if (debugOutput)
		printf("Set Video Mode, %lu modes; Current mode %lu is %s = %lu\n",
				count, currentMode, modes[currentMode].Name,
				modes[currentMode].Value);

	// Groups are:
	//   0 - width
	//   1 - height
	//   2 - format
	//   3 - fps
	std::regex reMode("([0-9]+)\\s*x\\s*([0-9]+)\\s+(.*?)\\s+([0-9.]+)\\s*fps");
	IMAQdxVideoMode* foundMode = nullptr;
	IMAQdxVideoMode* currentModePtr = &modes[currentMode];
	double foundFps = 1000.0;
	for (unsigned int i = 0; i < count; i++) {
		if (debugOutput)
			printf("Video mode[%u] is %s = %lu\n", i, modes[i].Name,
					modes[i].Value);
		std::cmatch m;
		if (!std::regex_match(modes[i].Name, m, reMode))
			continue;
		unsigned int width = (unsigned int) std::stoul(m[1].str());
		unsigned int height = (unsigned int) std::stoul(m[2].str());
		if (width != reqx)
			continue;
		if (height != reqy)
			continue;
		double fps = atof(m[4].str().c_str());
		if (fps < reqfps)
			continue;
		if (fps > foundFps)
			continue;
		bool isJpeg = m[3].str().compare("jpeg") == 0
				|| m[3].str().compare("JPEG") == 0;
		if ((use_Jpeg && !isJpeg) || (!use_Jpeg && isJpeg))
			continue;
		foundMode = &modes[i];
		foundFps = fps;
	}
	if (foundMode != nullptr) {
		if (foundMode->Value != currentModePtr->Value) {
			printf("Found mode is %s = %lu\n", foundMode->Name,
					foundMode->Value);
			IMAQdxSetAttribute(session, IMAQdxAttributeVideoMode,
					IMAQdxValueTypeU32, foundMode->Value);
			return true;
		}
	}
	return false;
}

// old SetMode
IMAQdxError Camera::SetMode() {
	uInt32 count;
	uInt32 mode;
	IMAQdxEnumerateVideoModes(session, NULL, &count, &mode);
	IMAQdxVideoMode videoModes[count];
	IMAQdxEnumerateVideoModes(session, videoModes, &count, &mode);
	for (uInt32 i = 0; i != count; i++) {
		//printf("Video mode[%lu] is %s = %lu\n", i, videoModes[i].Name, videoModes[i].Value);
		if (i == 95) {
			imaqError = IMAQdxSetAttribute(session, IMAQdxAttributeVideoMode,
					IMAQdxValueTypeU32, videoModes[i].Value);
			if (imaqError != IMAQdxErrorSuccess) {
				printf(
						"IMAQdxOpenCamera attempt to set video mode to %lu failed: %x\n",
						videoModes[i].Value, imaqError);
			}
			return imaqError;
		}
	}
	return IMAQdxErrorInternal;
}

void Camera::DumpAttrs() {
	//IMAQdxEnumerateAttributes(IMAQdxSession id, IMAQdxAttributeInformation attributeInformationArray[], uInt32* count, const char* root);
	IMAQdxAttributeInformation attributeInformationArray[300];
	uInt32 count;
	IMAQdxEnumerateAttributes(session, NULL, &count, "");
	union {
		uInt32 a;
		uInt64 b;
		double c;
		char *d;
		unsigned int e;bool f;
		char y[256];	// just in case
	} x;
	printf("IMAQdxEnumerateAttributes - there are %lu attributes", count);
	if (count
			> sizeof(attributeInformationArray)
					/ sizeof(attributeInformationArray[0])) {
		printf(" -- that's too many to enumerate\n");
	} else
		printf("\n");
	IMAQdxEnumerateAttributes(session, attributeInformationArray, &count, "");
	for (uInt32 i = 0; i != count; i++) {
		printf("  attr[%lu] is %s, type %u, writeable %lu, value=", i,
				attributeInformationArray[i].Name,
				attributeInformationArray[i].Type,
				attributeInformationArray[i].Writable);
		IMAQdxGetAttribute(session, attributeInformationArray[i].Name,
				(IMAQdxValueType) attributeInformationArray[i].Type, &x);
		switch (attributeInformationArray[i].Type) {
		case IMAQdxValueTypeU32:
			printf("%lu\n", x.a);
			break;
		case IMAQdxValueTypeI64:
			printf("%llu\n", x.b);
			break;
		case IMAQdxValueTypeF64:
			printf("%f\n", x.c);
			break;
		case IMAQdxValueTypeString:
			//printf("%s\n", x.d);
			printf("%s\n", x.y);
			break;
		case IMAQdxValueTypeEnumItem:
			printf("%u\n", x.e);
			break;
		case IMAQdxValueTypeBool:
			printf("%d\n", x.f);
			break;
		default:
			printf("%x\n", x.e);
			break;

		}
	}
}

IMAQdxError Camera::GetFrame() {
	imaqError = IMAQdxErrorSuccess;
	// grab an image,
	if (session != ULONG_MAX && frame != NULL) {
		if (false && firstTime) {
			printf("Dumping modes\n");
			DumpModes();
			printf("Dumping attrs\n");
			DumpAttrs();
		}
		IMAQdxGrab(session, frame, false, NULL);
		if (imaqError != IMAQdxErrorSuccess) {
			printf("IMAQdxConfigureGrab error (%s): %x\n",
					camInfo[camera].InterfaceName, imaqError);
		} else if (firstTime) {
			int xRes, yRes;
			imaqGetImageSize(frame, &xRes, &yRes);
			printf("Camera: first image grab is %d x %d frame=%x\n", xRes, yRes,
					(unsigned int) frame);
		}
		firstTime = false;
	}
	return imaqError;
}

