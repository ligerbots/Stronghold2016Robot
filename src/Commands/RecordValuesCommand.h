#ifndef RecordValuesCommand_H
#define RecordValuesCommand_H

#include "../CommandBase.h"
#include "WPILib.h"

class RecordValuesCommand: public CommandBase {
protected:
	FILE* dataFile;
public:
	RecordValuesCommand();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};

#endif
