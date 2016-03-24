#include <Stronghold2016Robot.h>
#include "RotateToTarget.h"

RotateToTarget::RotateToTarget() :
		// initialize with a dummy angle, and false for a relative turn
		RotateIMUCommand(0, false){}

void RotateToTarget::Initialize() {
	// Get the target angle from Vision. Will be zero if we have no target.
	angle = visionSubsystem->TargetAngle();
	printf("RotateToTarget: turn angle: %5.2f\n", angle);
	// let
	RotateIMUCommand::Initialize();
}
