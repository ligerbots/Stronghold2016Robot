#include <Stronghold2016Robot.h>

PDPSubsystem::PDPSubsystem() :
		Subsystem("PDPSubsystem") {
	mp_pdp.reset(new PowerDistributionPanel(RobotMap::PDP_CAN));
}

void PDPSubsystem::InitDefaultCommand() {
}

void PDPSubsystem::sendValuesToSmartDashboard() {
	// frequent polling of the pdp seems to be harmful to robot stability
	if (0 == Robot::ticks%25) {
		//double voltage = mp_pdp->GetVoltage();
		//SmartDashboard::PutNumber("PDP/Voltage", voltage);
		//SmartDashboard::PutNumber("PDP/TotalCurrent", mp_pdp->GetTotalCurrent());
		SmartDashboard::PutNumber("PDP/Watts", mp_pdp->GetTotalPower());
		SmartDashboard::PutNumber("PDP/Temperature", mp_pdp->GetTemperature());
		// Polling the individual channels overloads the CAN bus and spams the dashboard
//		for(int i=0;i<16;i++) {
//			std::string name = "PDP/Channel/";
//			name += std::to_string(i);
//			name += "_Current";
//			SmartDashboard::PutNumber(name, mp_pdp->GetCurrent(i));
//		}
	}
}

