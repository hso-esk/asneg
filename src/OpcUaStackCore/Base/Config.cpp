/*
   Copyright 2015-2017 Kai Huebl (kai@huebl-sgh.de)

   Lizenziert gemäß Apache Licence Version 2.0 (die „Lizenz“); Nutzung dieser
   Datei nur in Übereinstimmung mit der Lizenz erlaubt.
   Eine Kopie der Lizenz erhalten Sie auf http://www.apache.org/licenses/LICENSE-2.0.

   Sofern nicht gemäß geltendem Recht vorgeschrieben oder schriftlich vereinbart,
   erfolgt die Bereitstellung der im Rahmen der Lizenz verbreiteten Software OHNE
   GEWÄHR ODER VORBEHALTE – ganz gleich, ob ausdrücklich oder stillschweigend.

   Informationen über die jeweiligen Bedingungen für Genehmigungen und Einschränkungen
   im Rahmen der Lizenz finden Sie in der Lizenz.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#include <boost/algorithm/string.hpp>
#include "OpcUaStackCore/Base/Config.h"

namespace OpcUaStackCore
{

	Config* Config::instance_ = nullptr;
	Config::AliasMap Config::aliasMap_;
		
	Config* 
	Config::instance(void)
	{
		if (instance_ == nullptr) {
			instance_ = new Config();
		}
		return instance_;
	}

	void
	Config::destroy(void)
	{
		if (instance_ != nullptr) {
			instance_->clear();
			delete instance_;
			instance_ = nullptr;
		}
	}

	Config::Config(void)
	: ConfigIf()
	, child_()
	{
	}

	Config::Config(boost::property_tree::ptree& child)
	: ConfigIf()
	, child_(child)
	{
	}

	Config::~Config(void)
	{
	}

	void 
	Config::child(boost::property_tree::ptree& child)
	{
		child_ = child;
	}

	boost::property_tree::ptree& 
	Config::child(void)
	{
		return child_;
	}

	bool 
	Config::setValue(const std::string& value)
	{
		child_.put_value(value);
		return true;
	}

	bool
	Config::setValue(const std::string& path, const std::string& value)
	{
		boost::optional<std::string> val = getValue(path);
		if (val) return false;
		child_.put(path, value);
		return true;
	}

	bool 
	Config::setChild(const std::string& path, Config& config)
	{
		boost::optional<std::string> val = getValue(path);
		if (val) return false;

		child_.add_child(path, config.child());
		return true;
	}

	bool 
	Config::addValue(const std::string& path, const std::string& value)
	{
		child_.add(path, value);
		return true;
	}

	bool 
	Config::addChild(const std::string& path, Config& config)
	{
		boost::property_tree::ptree child = config.child();
		child_.add_child(path, child);
		return true;
	}

	std::string 
	Config::getValue(void)
	{
		return child_.data();
	}

	boost::optional<std::string>
	Config::getValue(const std::string& path)
	{
		boost::optional<std::string> value = child_.get_optional<std::string>(path);
		if (!value) return value;
		std::string substValue = *value;
		substAlias(substValue);
		return substValue;
	}
		
	std::string 
	Config::getValue(const std::string& path, const std::string& defaultValue)
	{
		boost::optional<std::string> value = getValue(path);
		if (!value) {
			std::string substValue = defaultValue;
			substAlias(substValue);
			return substValue;
		}
		std::string substValue = *value;
		substAlias(substValue);
		return substValue;
	}

	void 
	Config::getValues(const std::string& path, std::vector<std::string>& valueVec)
	{
		std::string valueName = path;
		size_t pos = path.find_last_of(".");
		if (pos == std::string::npos) {
			return getValuesFromName(valueName, valueVec);
		}

		valueName = path.substr(pos+1);
		std::string prefixPath = path.substr(0, pos);
		boost::optional<Config> cfg = getChild(prefixPath);
		if (!cfg) return;
		cfg->getValues(valueName, valueVec);
	}

	boost::optional<Config>
	Config::getChild(const std::string& path)
	{
		boost::optional<boost::property_tree::ptree&> child = child_.get_child_optional(path);
		if (!child) return boost::none;
		Config config(*child);
		return config;
	}

	void 
	Config::getChilds(const std::string& path, std::vector<Config>& configVec)
	{
		std::string valueName = path;
		size_t pos = path.find_last_of(".");
		if (pos == std::string::npos) {
			return getChildFromName(valueName, configVec);
		}

		valueName = path.substr(pos+1);
		std::string prefixPath = path.substr(0, pos);
		boost::optional<Config> cfg = getChild(prefixPath);
		if (!cfg) return;
		return cfg->getChilds(valueName, configVec);
	}

	bool 
	Config::exist(const std::string& path)
	{
		boost::optional<std::string> value = getValue(path);
		if (!value) return false;
		return true;
	}

	bool 
	Config::erase(const std::string& path)
	{
		boost::optional<boost::property_tree::ptree&> child = child_.get_child_optional(path);
		if (!child) return false;
		child_.erase(path);
		return true;
	}

    void
	Config::clear(void)
	{
		child_.clear();
	}

	void 
	Config::getValuesFromName(const std::string& valueName, std::vector<std::string>& valueVec)
	{
		boost::property_tree::ptree::iterator it;
		for (it = child_.begin(); it !=  child_.end(); it++) {
			if (it->first == valueName) {
				if (it->second.begin() != it->second.end()) continue;

				boost::optional<std::string> value = it->second.data();
				if (!value) continue;
				std::string substValue = *value;
				substAlias(substValue);
				valueVec.push_back(substValue);
			}
		}
	}

	void 
	Config::getChildFromName(const std::string& valueName, std::vector<Config>& valueVec)
	{
		boost::property_tree::ptree::iterator it;
		for (it = child_.begin(); it != child_.end(); it++) {
			if (it->first == valueName) {
				if (it->second.begin() == it->second.end()) continue;

				Config cfg(it->second);
				valueVec.push_back(cfg);
			}
		}
	}

	std::string
	Config::configFileName(void)
	{
		boost::optional<std::string> configFileName;
		configFileName = getValue("Global.ConfigurationFileName");
		if (!configFileName) return "unknwon";
		return *configFileName;
	}

	void
	Config::substAlias(std::string& value)
	{
		AliasMap::iterator it;
		for (it=aliasMap_.begin(); it!=aliasMap_.end(); it++) {
			boost::replace_all(value, it->first, it->second);
		}
	}

	bool
	Config::aliasExist(const std::string& aliasName)
	{
		AliasMap::iterator it;
		it = aliasMap_.find(aliasName);
		if (it == aliasMap_.end()) return false;
		return true;
	}

	void
	Config::alias(const std::string& aliasName, const std::string& aliasValue)
	{
		aliasMap_.erase(aliasName);
		aliasMap_.insert(std::make_pair(aliasName, aliasValue));
	}

	std::string
	Config::alias(const std::string& aliasName)
	{
		AliasMap::iterator it;
		it = aliasMap_.find(aliasName);
		if (it == aliasMap_.end()) return "";
		return it->second;
	}

	void
	Config::out(std::ostream& os)
	{
		out(os, child_, 0);
	}

	void
	Config::out(std::ostream& os,boost::property_tree::ptree& ptree, uint32_t depth)
	{
		// create prefix
		std::string prefix;
		for (uint32_t idx=0; idx<depth; idx++) {
			prefix += "  ";
		}

		// write data
		boost::property_tree::ptree::iterator it;
		for (it = ptree.begin(); it != ptree.end(); it++) {
			boost::optional<std::string> value = it->second.data();
			if (value) {
				os << prefix << it->first << " = " << *value << std::endl;
			}

			out(os, it->second, depth+1);
		}
	}

}
