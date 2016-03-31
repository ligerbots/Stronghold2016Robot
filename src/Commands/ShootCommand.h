#pragma once

#include <CommandBase.h>
#include <WPILib.h>


class ShootCommand: public CommandBase {
protected:
	int m_ticks;
	bool m_done;
public:
	explicit ShootCommand();
	void Initialize();
	void Execute();

	bool IsFinished();
	void End();
	void Interrupted();


};
