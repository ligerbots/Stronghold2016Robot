#include <Stronghold2016Robot.h>

WedgeSubsystem::WedgeSubsystem() :
		Subsystem("WedgeSubsystem")
{

}

void WedgeSubsystem::InitDefaultCommand()
{
	// Set the default command for a subsystem here.
	//SetDefaultCommand(new MySpecialCommand());
}

bool WedgeSubsystem::isWedgeDown() {
	return mp_wedgeDownSwitch->Get();
}
// Put methods for controlling this subsystem
// here. Call these from Commands.

