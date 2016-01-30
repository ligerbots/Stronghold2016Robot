#pragma once

/**
 * This interface exists for two reasons
 * - you can't use templated classes as template parameters
 *   (thus NTUpdater needs a map of IParameters instead of Parameter<whatever>s)
 * - To avoid circular includes on NTUpdater and Parameter
 */
class IParameter {
public:
	std::mutex lock;
	virtual void updateValue() = 0;
	virtual ~IParameter() {
	}
};
