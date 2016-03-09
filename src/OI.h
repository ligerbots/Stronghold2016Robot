#pragma once

#include "WPILib.h"

//cppcheck-suppress copyConstructor
class OI {
private:
	typedef std::map<uintptr_t, std::vector<JoystickButton*>> JoystickMap_t;
	typedef std::map<uintptr_t, std::vector<bool>> JoystickPressed_t;
	enum ButtonEvent {
		ACTIVE, INACTIVE, PRESSED, RELEASED
	};
	JoystickMap_t controllerButtons;
	JoystickPressed_t buttonsPressed;

public:
	Joystick* pXboxController;
	Joystick* pFarmController;
	bool m_secondControllerPresent;
	OI();
	OI(OI& otherOI);

	/**
	 * Registers commands to controller buttons
	 */
	void registerCommands();

	/**
	 * Returns true if the specified joystick button has been pressed and not released yet
	 * @param pJoystick The joystick to use
	 * @param button The button id (1-12)
	 */
	bool joystickButtonPressed(Joystick* pJoystick, int buttonNumber);

	/**
	 * Adds a single command to a joystick button. Keeps track of joystick buttons internally
	 * @param joystick The joystick to use
	 * @param buttonNumber The button number (1-12)
	 * @param when The ButtonEvent to attach the command to
	 * @param command The command
	 */
	void registerButton(Joystick* joystick, int buttonNumber, ButtonEvent when,
			Command* command);
	void registerSecondControllerButtons();
};
