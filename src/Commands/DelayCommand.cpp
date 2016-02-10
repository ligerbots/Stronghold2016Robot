#include <Stronghold2016Robot.h>

DelayCommand::DelayCommand(double delaySeconds) :
		m_ticks(0), m_delaySeconds(delaySeconds) {
//	Requires (delaySubsystem);
	// why is delaysubsystem necessary?
}

void DelayCommand::Initialize() {
	printf("Delay init'd for %1.2f seconds\n", m_delaySeconds);
	SetInterruptible(false);
	//SetTimeout(_delayPeriod);
	m_ticks = 0;
}

void DelayCommand::Execute() {
}

bool DelayCommand::IsFinished() {
	m_ticks++;
	double elapsedTime = m_ticks / RobotMap::TICKS_PER_SECOND;
	return elapsedTime >= m_delaySeconds;
	//return IsTimedOut();
}

void DelayCommand::End() {
	printf("Delay Done at %d\n", m_ticks);
	m_ticks = 0;
}

void DelayCommand::Interrupted() {
	printf("Delay INTERRUPTED at %d\n", m_ticks);
}
