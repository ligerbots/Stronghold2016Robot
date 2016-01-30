#include <Stronghold2016Robot.h>

// specialized methods
template<> Parameter<int>::Parameter() {
	printf("Initializing empty parameter\n");
	key = "";
	value = 0;
}

template<> Parameter<double>::Parameter() {
	printf("Initializing empty parameter\n");
	key = "";
	value = 0;
}

template<> Parameter<bool>::Parameter() {
	printf("Initializing empty parameter\n");
	key = "";
	value = false;
}

template<> Parameter<std::string>::Parameter() {
	printf("Initializing empty parameter\n");
	key = "";
	value = "";
}

template<> void Parameter<int>::set(int val) {
	value = val;
	Preferences::GetInstance()->PutInt(key, value);
}

template<> void Parameter<double>::set(double val) {
	value = val;
	Preferences::GetInstance()->PutDouble(key, value);
}

template<> void Parameter<bool>::set(bool val) {
	value = val;
	Preferences::GetInstance()->PutBoolean(key, value);
}

template<> void Parameter<std::string>::set(std::string val) {
	value = val;
	Preferences::GetInstance()->PutString(key, value);
}

template<> void Parameter<int>::updateValue() {
	value = Preferences::GetInstance()->GetInt(key, value);
}

template<> void Parameter<double>::updateValue() {
	value = Preferences::GetInstance()->GetDouble(key, value);
}

template<> void Parameter<bool>::updateValue() {
	value = Preferences::GetInstance()->GetBoolean(key, value);
}

template<> void Parameter<std::string>::updateValue() {
	value = Preferences::GetInstance()->GetString(key, value);
}
