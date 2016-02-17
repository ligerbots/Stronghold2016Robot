#pragma once

#include <CommandBase.h>
#include <WPILib.h>

//Basic command to copy

class TabulaRasaCommand: public CommandBase {
protected:

public:
	explicit TabulaRasaCommand();
	void Initialize();
	void Execute();

	bool IsFinished();
	void End();
	void Interrupted();


};
