#pragma once

#include <WPILib.h>
#include "IParameter.h"
#include "NTUpdater.h"

/**
 * Parameter is a class to effectively manage preferences that would normally
 * be used through WPILib Preferences. The advantage of using Parameter is that
 * it will be notified of updates from the SmartDashboard so the program doesn't
 * have to restart
 * Instead of
 * <pre><code>int myInt = Preferences::GetInstance()->GetInt("MyInt")</code></pre>
 * use
 * <pre><code>Parameter<int> myInt("MyInt");</code></pre>
 *
 * Parameter also includes many overriden operator functions, so you can
 * directly use operators on Parameters and have them update on file and in network tables
 */
template<typename Parameter_Type> class Parameter: public IParameter {
protected:
	std::string key;
	Parameter_Type value;
	void initialize(std::string key) {
		printf(("Initializing new parameter for " + key + "\n").c_str());
		this->key = key;
		// initialize preferences
		Preferences::GetInstance();
		// initialize the NetworkTables listener that notifies this object when an update is sent
		NTUpdater::registerNTUpdater();
		// add this parameter to the list of parameters to notify
		IParameter* IThis = this;
		NTUpdater::addParameter(key, std::shared_ptr<IParameter>(IThis));
		updateValue();
	}
public:
	Parameter();
	explicit Parameter(std::string key) :
			key(key) {
		printf(("Constructing a new Parameter with key " + key + "\n").c_str());
		initialize(key);
	}

	explicit Parameter(const Parameter& copyFrom) :
			key(copyFrom.key) {
		initialize(key);
	}

	virtual ~Parameter() {
		// make sure NTUpdater isn't notifying garbage pointers
		IParameter* IThis = this;
		NTUpdater::removeParameter(key, std::shared_ptr<IParameter>(IThis));
	}

	void set(Parameter_Type val);
	void updateValue();

	Parameter_Type& get() {
		return value;
	}

	Parameter& operator=(const Parameter_Type& other) {
		std::lock_guard<std::mutex> guard(lock);
		set(other);
		return *this;
	}

	Parameter& operator=(const Parameter& other){
		std::lock_guard<std::mutex> guard(lock);
		set(other.value);
		return *this;
	}

	Parameter& operator+=(const Parameter_Type& other) {
		std::lock_guard<std::mutex> guard(lock);
		updateValue();
		set(value + other);
		return *this;
	}
	Parameter& operator-=(const Parameter_Type& other) {
		std::lock_guard<std::mutex> guard(lock);
		updateValue();
		set(value - other);
		return *this;
	}
	Parameter& operator*=(const Parameter_Type& other) {
		std::lock_guard<std::mutex> guard(lock);
		updateValue();
		set(value * other);
		return *this;
	}
	Parameter& operator/=(const Parameter_Type& other) {
		std::lock_guard<std::mutex> guard(lock);
		updateValue();
		set(value / other);
		return *this;
	}

	Parameter& operator++() {
		std::lock_guard<std::mutex> guard(lock);
		updateValue();
		set(++value);
		return *this;
	}
	Parameter& operator--() {
		std::lock_guard<std::mutex> guard(lock);
		updateValue();
		set(--value);
		return *this;
	}

	Parameter_Type operator+(const Parameter_Type& other) const {
		return value + other;
	}

	Parameter_Type operator-(const Parameter_Type& other) const {
		return value - other;
	}

	Parameter_Type operator*(const Parameter_Type& other) const {
		return value * other;
	}

	Parameter_Type operator/(const Parameter_Type& other) const {
		return value / other;
	}

	bool operator==(const Parameter_Type& other) const {
		return value == other;
	}

	bool operator==(const Parameter& other) const {
		return this->value == other.value;
	}

	bool operator!=(const Parameter_Type& other) const {
		return value != other;
	}

	bool operator!=(const Parameter& other) const {
		return this->value != other.value;
	}

	bool operator!() const {
		return !value;
	}
};
