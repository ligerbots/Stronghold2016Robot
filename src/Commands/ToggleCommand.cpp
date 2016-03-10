#include "ToggleCommand.h"

ToggleCommand::ToggleCommand(Command* whatToCall) :
		whatToCall(whatToCall) {
}

void ToggleCommand::Initialize() {
	if(whatToCall->IsRunning()){
		whatToCall->Cancel();
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
