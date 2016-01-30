#include <Stronghold2016Robot.h>

std::string NTUpdater::TABLE_NAME = "Preferences";
NTUpdater* NTUpdater::instance = NULL;
std::shared_ptr<NetworkTable> NTUpdater::NETWORK_TABLE;
std::map<std::string, NTParameterVector_t> NTUpdater::parametersMap;
std::mutex NTUpdater::lock;

NTUpdater::NTUpdater() {
}

NTUpdater::~NTUpdater() {
}

void NTUpdater::registerNTUpdater() {
	if (instance != NULL)
		return;
	printf("Initializing Parameter updater\n");
	instance = new NTUpdater();
	NETWORK_TABLE = NetworkTable::GetTable(TABLE_NAME);
	NETWORK_TABLE->AddTableListener(instance);
}

void NTUpdater::addParameter(std::string key,
		std::shared_ptr<IParameter> parameter) {
	std::lock_guard<std::mutex> guard(lock);
	std::map<std::string, NTParameterVector_t>::iterator it = parametersMap.find(
			key);
	if (it == parametersMap.end()) {
		NTParameterVector_t parameterVector;
		parameterVector.push_back(parameter);
		parametersMap.insert(std::make_pair(key, parameterVector));
	} else {
		it->second.push_back(parameter);
	}
}

void NTUpdater::removeParameter(std::string key,
		std::shared_ptr<IParameter> parameter) {
	std::lock_guard<std::mutex> guard(lock);
	std::map<std::string, NTParameterVector_t>::iterator it = parametersMap.find(
			key);
	if (it != parametersMap.end()) {
		it->second.erase(
				std::remove(it->second.begin(), it->second.end(), parameter),
				it->second.end());
	}
}

void NTUpdater::ValueChanged(ITable* source, llvm::StringRef key,
		std::shared_ptr<nt::Value> value,
		bool isNew) {
	std::map<std::string, NTParameterVector_t>::iterator it = parametersMap.find(
			key);
	if (it != parametersMap.end()) {
		for (NTParameterVector_t::iterator it2 = it->second.begin();
				it2 != it->second.end(); ++it2) {
			std::lock_guard<std::mutex> guard((*it2)->lock);
			(*it2)->updateValue();
		}
	}
}
