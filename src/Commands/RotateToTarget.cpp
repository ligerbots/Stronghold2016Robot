#include <Stronghold2016Robot.h>
#include "RotateToTarget.h"

// the majority of the logic for this is implemented by RotateIMUCommand
// this just calculates and sets the target angle on Initialize()
RotateToTarget::RotateToTarget() :
		// initialize with a dummy angle, and false for a relative turn
		RotateIMUCommand(0, false){}

void RotateToTarget::Initialize() {
	// Get the target angle from Vision. Will be zero if we have no target.
	m_angle = -(visionSubsystem->TargetAngle());
	printf("RotateToTarget: turn angle: %5.2f\n", m_angle);
	// let parent command initialize with the angle set
	RotateIMUCommand::Initialize();
}
