#pragma once

#include <WPILib.h>
#include "llvm/StringRef.h"
#include "IParameter.h"

typedef std::vector<std::shared_ptr<IParameter>> NTParameterVector_t;

class NTUpdater: public ITableListener {
protected:
	static NTUpdater* instance;
	static std::map<std::string, NTParameterVector_t> parametersMap;
	static std::mutex lock;
public:
	static std::string TABLE_NAME;
	static std::shared_ptr<NetworkTable> NETWORK_TABLE;
	static std::string SMARTDASHBOARD_TABLE_NAME;
	static std::shared_ptr<NetworkTable> SMARTDASHBOARD_NETWORK_TABLE;

	NTUpdater();
	virtual ~NTUpdater();
	void ValueChanged(ITable* source,
	                            llvm::StringRef key,
	                            std::shared_ptr<nt::Value> value,
	                            bool isNew);
	static void registerNTUpdater();
	static void addParameter(std::string key, std::shared_ptr<IParameter> parameter);
	static void removeParameter(std::string key, std::shared_ptr<IParameter> parameter);
};
