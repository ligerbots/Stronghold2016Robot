#include <Stronghold2016Robot.h>

PDPSubsystem::PDPSubsystem() :
		Subsystem("ExampleSubsystem") {
	mp_pdp.reset(new PowerDistributionPanel(RobotMap::PDP_CAN));
//	mp_pdp->StartLiveWindowMode();
}

void PDPSubsystem::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	//SetDefaultCommand(new MySpecialCommand());
}

void PDPSubsystem::sendValuesToSmartDashboard(){
	for(int i=0;i<16;i++){
		std::string name = "PDP/Channel/";
		name += std::to_string(i);
		name += "_Current";
		SmartDashboard::PutNumber(name, mp_pdp->GetCurrent(i));
	}
	SmartDashboard::PutNumber("PDP/TotalCurrent", mp_pdp->GetTotalCurrent());
	SmartDashboard::PutNumber("PDP/Voltage", mp_pdp->GetVoltage());
	SmartDashboard::PutNumber("PDP/Watts", mp_pdp->GetTotalPower());
	SmartDashboard::PutNumber("PDP/Temperature", mp_pdp->GetTemperature());
}
