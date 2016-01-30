#include <Stronghold2016Robot.h>

OI::OI() :
		controllerButtons(), buttonsPressed() {
	pXboxController = new Joystick(0);
	pLogitechJoystick = new Joystick(1);
	// TODO: register commands here
}

bool OI::joystickButtonPressed(Joystick* pJoystick, int buttonNumber){
	bool pressed = false;

	// find the vector to use
	JoystickPressed_t::iterator buttonsPressedIterator = buttonsPressed.find((uintptr_t) pJoystick);
	if (buttonsPressedIterator == controllerButtons.end()) {
		std::vector<bool> apButtonsPressed;
		buttonsPressed.insert(
				std::pair<uintptr_t, std::vector<bool>>(
						(uintptr_t) pJoystick, apButtonsPressed));
	}

	buttonsPressedIterator = controllerButtons.find((uintptr_t) pJoystick);

	// check if index is there
	if (buttonsPressedIterator->second.at(buttonNumber) != NULL) {
		pressed = buttonsPressedIterator->second.at(buttonNumber);
	} else {
		pressed = false;
		buttonsPressedIterator->second.at(buttonNumber) = pressed;
	}

	bool joystickPressed = pJoystick->GetRawButton(buttonNumber);
	bool returnValue = false;
	if(joystickPressed && !pressed){
		returnValue = true;
	}

	buttonsPressedIterator->second.at(buttonNumber) = joystickPressed;

	return returnValue;
}

void OI::registerButton(Joystick* pJoystick, int buttonNumber, ButtonEvent when,
		Command* pCommand) {
	JoystickButton* button = NULL;

	// find the vector to use
	JoystickMap_t::iterator cIt = controllerButtons.find((uintptr_t) pJoystick);
	if (cIt == controllerButtons.end()) {
		std::vector<JoystickButton*> apButtons(13);
		controllerButtons.insert(
				std::pair<uintptr_t, std::vector<JoystickButton*>>(
						(uintptr_t) pJoystick, apButtons));
	}

	cIt = controllerButtons.find((uintptr_t) pJoystick);

	// check if joystick is there
	if (cIt->second.at(buttonNumber) != NULL) {
		button = cIt->second.at(buttonNumber);
	} else {
		button = new JoystickButton(pJoystick, buttonNumber);
		cIt->second.at(buttonNumber) = button;
	}

	// attach the command to the correct event
	switch (when) {
	case ACTIVE:
		button->WhenActive(pCommand);
		break;
	case INACTIVE:
		button->WhenInactive(pCommand);
		break;
	case PRESSED:
		button->WhenPressed(pCommand);
		break;
	case RELEASED:
		button->WhenReleased(pCommand);
		break;
	}
}
