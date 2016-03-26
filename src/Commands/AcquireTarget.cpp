#include "AcquireTarget.h"

AcquireTarget::AcquireTarget(bool waitForVision, bool forceUpdate) :
				CommandBase("AcquireTarget") {
	m_waitForVision = waitForVision;
	m_forceUpdate = forceUpdate;
	m_isRunningVision = true;
}

void AcquireTarget::Initialize() {
	printf("Acquire Target\n");
	// if the robot has moved and the calculation is invalid, get another frame
	// otherwise skip it
	m_isRunningVision = m_forceUpdate || visionSubsystem->isVisionCalculationDirty();
	if(m_isRunningVision){
		visionSubsystem->runVision();
	} else {
		printf("AcquireTarget: skipping vision frame because position hasn't changed\n");
	}
}

void AcquireTarget::Execute() {

}

bool AcquireTarget::IsFinished() {
	if(!m_isRunningVision){
		return true; // the calculation is still valid, we didn't bother to get another frame
	} else if(m_waitForVision){
		bool visionBusy = visionSubsystem->isVisionBusy(); // wait until the thread finishes if this command is set to wait
		printf("AcquireTarget: vision busy = %d\n", visionBusy);
		return !visionBusy;
	} else {
		return true; // otherwise exit immediately; don't wait for completion
	}
}

void AcquireTarget::End() {

}

void AcquireTarget::Interrupted() {

}
