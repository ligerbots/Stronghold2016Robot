#include <Stronghold2016Robot.h>

// specialized methods
template<> Parameter<int>::Parameter() {
	printf("Initializing empty parameter\n");
	key = "";
	value = 0;
	persist = false;
}

template<> Parameter<double>::Parameter() {
	printf("Initializing empty parameter\n");
	key = "";
	value = 0;
	persist = false;
}

template<> Parameter<bool>::Parameter() {
	printf("Initializing empty parameter\n");
	key = "";
	value = false;
	persist = false;
}

template<> Parameter<std::string>::Parameter() {
	printf("Initializing empty parameter\n");
	key = "";
	value = "";
	persist = false;
}

template<> void Parameter<int>::set(int val) {
	value = val;
	if (persist)
		Preferences::GetInstance()->PutInt(key, value);
	else
		SmartDashboard::PutNumber(key, value);
}

template<> void Parameter<double>::set(double val) {
	value = val;
	if (persist)
		Preferences::GetInstance()->PutDouble(key, value);
	else
		SmartDashboard::PutNumber(key, value);
}

template<> void Parameter<bool>::set(bool val) {
	value = val;
	if (persist)
		Preferences::GetInstance()->PutBoolean(key, value);
	else
		SmartDashboard::PutBoolean(key, value);
}

template<> void Parameter<std::string>::set(std::string val) {
	value = val;
	if (persist)
		Preferences::GetInstance()->PutString(key, value);
	else
		SmartDashboard::PutString(key, value);
}

template<> void Parameter<int>::updateValue() {
	if (persist)
		value = Preferences::GetInstance()->GetInt(key, value);
	else
		value = SmartDashboard::GetNumber(key, value);
}

template<> void Parameter<double>::updateValue() {
	if (persist)
		value = Preferences::GetInstance()->GetDouble(key, value);
	else
		value = SmartDashboard::GetNumber(key, value);
}

template<> void Parameter<bool>::updateValue() {
	if (persist)
		value = Preferences::GetInstance()->GetBoolean(key, value);
	else
		value = SmartDashboard::GetBoolean(key, value);
}

template<> void Parameter<std::string>::updateValue() {
	if (persist)
		value = Preferences::GetInstance()->GetString(key, value);
	else
		value = SmartDashboard::GetString(key, value);
}
