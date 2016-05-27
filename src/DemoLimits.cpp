#include <DemoLimits.h>
#include <WPILib.h>

void DemoLimits::Init(){
	if(!Preferences::GetInstance()->ContainsKey("Demo Limits Speed")){
		Preferences::GetInstance()->PutDouble("Demo Limits Speed", 1);
	}
	if(!Preferences::GetInstance()->ContainsKey("Demo Limits Auto Shooting")){
		Preferences::GetInstance()->PutBoolean("Demo Limits Auto Shooting", true);
	}
	if(!Preferences::GetInstance()->ContainsKey("Demo Limits Manual Shooting")){
		Preferences::GetInstance()->PutBoolean("Demo Limits Manual Shooting", true);
	}
	if(!Preferences::GetInstance()->ContainsKey("Demo Limits Manual Intake")){
		Preferences::GetInstance()->PutBoolean("Demo Limits Manual Intake", true);
	}
}

double DemoLimits::GetMaxSpeed(){
	return Preferences::GetInstance()->GetDouble("Demo Limits Speed", 1);
}

bool DemoLimits::IsAutoShootingAllowed(){
	return Preferences::GetInstance()->GetBoolean("Demo Limits Auto Shooting", true);
}

bool DemoLimits::IsManualShootingAllowed(){
	return Preferences::GetInstance()->GetBoolean("Demo Limits Manual Shooting", true);
}

bool DemoLimits::IsManualIntakeAllowed(){
	return Preferences::GetInstance()->GetBoolean("DemoLimits Manual Intake", true);
}
