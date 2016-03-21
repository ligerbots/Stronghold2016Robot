#include "AcquireTarget.h"

AcquireTarget::AcquireTarget() : CommandBase("AcquireTarget")
{
	// Use Requires() here to declare subsystem dependencies
	// eg. Requires(chassis);
}

// Called just before this Command runs the first time
void AcquireTarget::Initialize()
{
	visionSubsystem->runVision();
}

// Called repeatedly when this Command is scheduled to run
void AcquireTarget::Execute()
{

}

// Make this return true when this Command no longer needs to run execute()
bool AcquireTarget::IsFinished()
{
	return true;
}

// Called once after isFinished returns true
void AcquireTarget::End()
{

}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void AcquireTarget::Interrupted()
{

}
