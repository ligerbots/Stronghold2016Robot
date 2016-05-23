#include <DemoLimits.h>
#include <WPILib.h>

double DemoLimits::GetMaxSpeed(){
	return SmartDashboard::GetNumber("Demo Limits/Speed", 1);
}

bool DemoLimits::IsAutoShootingAllowed(){
	return SmartDashboard::GetBoolean("Demo Limits/Auto Shooting", true);
}

bool DemoLimits::IsManualShootingAllowed(){
	return SmartDashboard::GetBoolean("Demo Limits/Manual Shooting", true);
}

bool DemoLimits::IsManualIntakeAllowed(){
	return SmartDashboard::GetBoolean("DemoLimits/Manual Intake", true);
}
