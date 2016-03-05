/*
 * Camaera.h
 *
 *  Created on: Mar 9, 2015
 *      Author: cbf
 */

#pragma once
#include <NIIMAQdx.h>
#include <nivision.h>

class Camera {
private:
	static IMAQdxCameraInformation camInfo[6];
	static Camera *cameras[6];	// array of ourselves
	static uInt32 cameraCount;
	static uInt32 currentCamera;
	static bool debugOutput;
	Image *frame;
	IMAQdxSession session;
	IMAQdxError imaqError;
	uInt32 camera;
	bool firstTime;
	bool verticalFlip;
	int frameWidth;
	int frameHeight;
	static bool enabled;
	static bool overlay;

public:
	explicit Camera(uInt32 i);
	void SetVerticalFlip(bool flip);
	void DumpAttrs();
	void DumpModes();
	IMAQdxError SetMode();
	bool SetVideoMode(unsigned int x, unsigned int y, unsigned int fps, bool useJpeg);
	void SetExposure(int percent);
	int GetWidth() const;
	int GetHeight() const;
	void DumpExposureSettings();
	static Camera* GetCamera(int number);
	static int GetNumberOfCameras();
	static int EnumerateCameras();
	static uInt32 SwitchCamera(uInt32 which);
	static void StartCameras();
	static void StopCameras();
	static void Feed(int ticks);
	static char *GetName();
	static char *GetModel();
	static void EnableCameras();
	static void DisableCameras();
	static bool IsEnabled();
	static bool IsOpen();
	static void ToggleOverlay();

	/**
	 * Returns the stored frame from the last capture
	 * @return The image
	 */
	Image* GetStoredFrame();

	IMAQdxError Start();
	IMAQdxError GetFrame();
	IMAQdxError Stop();

};
