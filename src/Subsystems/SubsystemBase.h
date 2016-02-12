/*
 * SmartDashboardSubsystem.h
 *
 *  Created on: Feb 11, 2016
 *      Author: Erik
 */

#pragma once

#include <WPILib.h>

/**
 * Possibly what some or all subsystems can extend instead of plain Subsystem
 */
class SubsystemBase: public Subsystem {
public:
	explicit SubsystemBase(const std::string &name) :
			Subsystem(name) {
	}
	virtual void sendValuesToSmartDashboard() = 0;
};

