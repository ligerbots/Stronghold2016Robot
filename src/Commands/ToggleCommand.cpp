#include "ToggleCommand.h"

ToggleCommand::ToggleCommand(Command* whatToCall) :
		whatToCall(whatToCall) {
}

ToggleCommand::ToggleCommand(Command* whatToCall, std::vector<Command*> whatToRestart) :
		whatToCall(whatToCall), ma_restartCommands(whatToRestart) {
}

void ToggleCommand::Initialize() {
	if(whatToCall->IsRunning()){
		whatToCall->Cancel();
		for(std::vector<Command*>::iterator it = ma_restartCommands.begin(); it != ma_restartCommands.end(); ++it){
			(*it)->Start();
		}
	} else {
		whatToCall->Start();
	}
}

void ToggleCommand::Execute() {
}

bool ToggleCommand::IsFinished() {
	return true;
}

void ToggleCommand::End() {
}

void ToggleCommand::Interrupted() {
}
