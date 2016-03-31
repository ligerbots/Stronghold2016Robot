#ifndef RotateIMUCommand_H
#define RotateIMUCommand_H

#include "../CommandBase.h"
#include "WPILib.h"

class RotateIMUCommand: public CommandBase {
protected:
	double m_angle;
	double m_targetAngle;
	double m_currentAngle;
	double m_lastAngle;
	bool m_isClockwise;
	bool m_isAbsolute;
	int m_ticks;

	void updateCurrentAngle();
public:
	static constexpr double MIN_SPEED = .38;
	static constexpr double RAMP_UP_TICKS = 20;
	static constexpr double RAMP_DOWN_ZONE = 40;

	explicit RotateIMUCommand(double targetAngle, bool absolute = true);
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

#endif
