#ifndef PDPSubsystem_H
#define PDPSubsystem_H

#include "Commands/Subsystem.h"
#include "WPILib.h"

class PDPSubsystem: public Subsystem
{
private:
	std::unique_ptr<PowerDistributionPanel> mp_pdp;
public:
	PDPSubsystem();
	void InitDefaultCommand();
	void sendValuesToSmartDashboard();
};

#endif
