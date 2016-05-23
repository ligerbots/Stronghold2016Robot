#pragma once

class DemoLimits {
public:

	/**
	 * (especially on high gear) the maximum speed of the drive is dangerously high
	 */
	static double GetMaxSpeed();

	/**
	 * We may want to disallow shooting altogether
	 */
	static bool IsAutoShootingAllowed();

	/**
	 * I'd trust software to set things correctly more than people
	 */
	static bool IsManualShootingAllowed();

	/**
	 * Can damage flaps if not used correctly
	 */
	static bool IsManualIntakeAllowed();
};

