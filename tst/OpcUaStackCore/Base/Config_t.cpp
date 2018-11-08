#include "unittest.h"
#include "OpcUaStackCore/Base/Config.h"
#include <iostream>

using namespace OpcUaStackCore;

BOOST_AUTO_TEST_SUITE(Config_t)

BOOST_AUTO_TEST_CASE(Config_)
{
	std::cout << "Config_t" << std::endl;
}

BOOST_AUTO_TEST_CASE(Config_setValue_getValue)
{
	Config cfg;
	
	BOOST_REQUIRE(cfg.setValue("p1", "v1") == true);
	BOOST_REQUIRE(cfg.setValue("p2.p3.p4", "v234") == true);
	BOOST_REQUIRE(cfg.setValue("p2.p5", "v25") == true);
	BOOST_REQUIRE(cfg.setValue("p2.p5", "v25") == false);
	BOOST_REQUIRE(!cfg.getValue("path.not.exist") == true);

	BOOST_REQUIRE(!cfg.getValue("p2.p5") == false);
	BOOST_REQUIRE(*cfg.getValue("p2.p5") == "v25");

	BOOST_REQUIRE(cfg.getValue("path.not.exist", "default") == "default");
	BOOST_REQUIRE(cfg.getValue("p2.p5", "default") == "v25");
}

BOOST_AUTO_TEST_CASE(Config_exist)
{
	Config cfg;

	BOOST_REQUIRE(cfg.setValue("p1", "v1") == true);
	BOOST_REQUIRE(cfg.setValue("p2.p3.p4", "v234") == true);
	BOOST_REQUIRE(cfg.setValue("p2.p3.p6", "v234") == true);
	BOOST_REQUIRE(cfg.setValue("p2.p5", "v25") == true);
	BOOST_REQUIRE(cfg.setValue("p2.p5", "v25") == false);

	BOOST_REQUIRE(cfg.exist("value.not.exist") == false);
	BOOST_REQUIRE(cfg.exist("p2") == true);
	BOOST_REQUIRE(cfg.exist("p2.p3.p4") == true);
}

BOOST_AUTO_TEST_CASE(Config_getChild)
{
	Config cfg1;
	boost::optional<Config> cfg2;
	
	BOOST_REQUIRE(cfg1.setValue("p1", "v1") == true);
	BOOST_REQUIRE(cfg1.setValue("p2.p3.p1", "v231") == true);
	BOOST_REQUIRE(cfg1.setValue("p2.p3.p2", "v232") == true);
	BOOST_REQUIRE(cfg1.setValue("p2.p5", "v25") == true);
	BOOST_REQUIRE(cfg1.setValue("p2.p5", "v25") == false);
	BOOST_REQUIRE(!cfg1.getValue("path.not.exist") == true);

	BOOST_REQUIRE(!cfg1.getChild("child.not.exist") == true);
	BOOST_REQUIRE(!cfg1.getChild("p1") == false);
	BOOST_REQUIRE(!cfg1.getChild("p2") == false);
	BOOST_REQUIRE(!cfg1.getChild("p2.p3.p1") == false);
	BOOST_REQUIRE(!cfg1.getChild("p2.p3") == false);

	cfg2 = cfg1.getChild("p1");
	BOOST_REQUIRE(cfg2->getValue() == "v1");

	cfg2 = cfg1.getChild("p2");
	BOOST_REQUIRE(*cfg2->getValue("p3.p1") == "v231");
	BOOST_REQUIRE(*cfg2->getValue("p3.p2") == "v232");

	cfg2 = cfg1.getChild("p2.p3.p1");
	BOOST_REQUIRE(cfg2->getValue() == "v231");

	cfg2 = cfg1.getChild("p2.p3");
	BOOST_REQUIRE(*cfg2->getValue("p1") == "v231");
	BOOST_REQUIRE(*cfg2->getValue("p2") == "v232");
}

BOOST_AUTO_TEST_CASE(Config_setChild)
{
	Config cfg1, cfg2;
	
	BOOST_REQUIRE(cfg1.setValue("p1", "v1") == true);
	BOOST_REQUIRE(cfg1.setValue("p2.p3.p1", "v231") == true);
	BOOST_REQUIRE(cfg1.setValue("p2.p3.p2", "v232") == true);
	BOOST_REQUIRE(cfg1.setValue("p2.p5", "v25") == true);
	BOOST_REQUIRE(cfg1.setValue("p2.p5", "v25") == false);
	BOOST_REQUIRE(!cfg1.getValue("path.not.exist") == true);

	BOOST_REQUIRE(cfg2.setValue("p1", "v1") == true);
	BOOST_REQUIRE(cfg2.setValue("p2.p3.p1", "v231") == true);
	BOOST_REQUIRE(cfg2.setValue("p2.p3.p2", "v232") == true);
	BOOST_REQUIRE(cfg2.setValue("p2.p5", "v25") == true);
	BOOST_REQUIRE(cfg2.setValue("p2.p5", "v25") == false);
	BOOST_REQUIRE(!cfg2.getValue("path.not.exist") == true);

	BOOST_REQUIRE(cfg1.setChild("p3", cfg2) == true);
	BOOST_REQUIRE(cfg1.getValue("p1", "default") == "v1");
	BOOST_REQUIRE(cfg1.getValue("p3.p1", "default") == "v1");
}

BOOST_AUTO_TEST_CASE(Config_dublicate_value)
{
	Config cfg1;
	
	BOOST_REQUIRE(cfg1.setValue("p1", "v1") == true);
	BOOST_REQUIRE(cfg1.getValue("p1", "default") == "v1");
	BOOST_REQUIRE(cfg1.setValue("p1", "v2") == false);
	BOOST_REQUIRE(cfg1.getValue("p1", "default") == "v1");
}

BOOST_AUTO_TEST_CASE(Config_dublicate_child)
{
	Config cfg1, cfg2;
	
	BOOST_REQUIRE(cfg1.setValue("p1", "v1") == true);
	BOOST_REQUIRE(cfg2.setValue("p1", "v2") == true);

	BOOST_REQUIRE(cfg1.setChild("p2", cfg2) == true);
	BOOST_REQUIRE(cfg1.getValue("p2.p1", "default") == "v2");

	BOOST_REQUIRE(cfg1.setChild("p2", cfg2) == false);
	BOOST_REQUIRE(cfg1.getValue("p2.p1", "default") == "v2");
}

BOOST_AUTO_TEST_CASE(Config_erase)
{
	Config cfg;
	
	BOOST_REQUIRE(cfg.setValue("p1", "v1") == true);
	BOOST_REQUIRE(cfg.setValue("p2.p21", "v21") == true);
	BOOST_REQUIRE(cfg.setValue("p2.p22", "v22") == true);

	BOOST_REQUIRE(cfg.exist("p1") == true);
	BOOST_REQUIRE(cfg.exist("p2.p21") == true);
	BOOST_REQUIRE(cfg.exist("p2.p22") == true);

	BOOST_REQUIRE(cfg.erase("p1") == true);
	BOOST_REQUIRE(cfg.exist("p1") == false);
	BOOST_REQUIRE(cfg.exist("p2") == true);
	BOOST_REQUIRE(cfg.exist("p2.p21") == true);
	BOOST_REQUIRE(cfg.exist("p2.p22") == true);

	BOOST_REQUIRE(cfg.erase("p2") == true);
	BOOST_REQUIRE(cfg.exist("p1") == false);
	BOOST_REQUIRE(cfg.exist("p2") == false);
	BOOST_REQUIRE(cfg.exist("p2.p21") == false);
	BOOST_REQUIRE(cfg.exist("p2.p22") == false);

	BOOST_REQUIRE(cfg.erase("p1") == false);
	BOOST_REQUIRE(cfg.erase("p2") == false);
}

BOOST_AUTO_TEST_CASE(Config_getConfigParameter)
{
	Config cfg1;
	boost::optional<Config> cfg2;

	uint32_t numberValue;
	std::string stringValue;
	
	BOOST_REQUIRE(cfg1.setValue("p1", "v1") == true);
	BOOST_REQUIRE(cfg1.setValue("p2.p21", "123") == true);
	BOOST_REQUIRE(cfg1.setValue("p2.p22", "v22") == true);

	BOOST_REQUIRE(cfg1.getConfigParameter("value.gibts.nicht", stringValue) == false);
	BOOST_REQUIRE(cfg1.getConfigParameter("p1", numberValue) == false);
	BOOST_REQUIRE(cfg1.getConfigParameter("p1", stringValue) == true);
	BOOST_REQUIRE(stringValue == "v1");
	BOOST_REQUIRE(cfg1.getConfigParameter("p2.p21", numberValue) == true);
	BOOST_REQUIRE(numberValue == 123);

	BOOST_REQUIRE(cfg1.getConfigParameter("value.gibts.nicht", numberValue, "defaultValue") == false);
	BOOST_REQUIRE(cfg1.getConfigParameter("value.gibts.nicht", stringValue, "defaultValue") == true);
	BOOST_REQUIRE(stringValue == "defaultValue");
	BOOST_REQUIRE(cfg1.getConfigParameter("value.gibts.nicht", numberValue, "123") == true);
	BOOST_REQUIRE(numberValue == 123);
	BOOST_REQUIRE(cfg1.getConfigParameter("value.gibts.nicht", stringValue, "123") == true);
	BOOST_REQUIRE(stringValue == "123");

	BOOST_REQUIRE(cfg1.getConfigParameter("p1", numberValue, "defaultValue") == false);
	BOOST_REQUIRE(cfg1.getConfigParameter("p1", stringValue, "defaultValue") == true);
	BOOST_REQUIRE(stringValue == "v1");
	BOOST_REQUIRE(cfg1.getConfigParameter("p1", numberValue, "123") == true);
	BOOST_REQUIRE(numberValue == 123);
	BOOST_REQUIRE(cfg1.getConfigParameter("p1", stringValue, "123") == true);
	BOOST_REQUIRE(stringValue == "v1");

	BOOST_REQUIRE(cfg1.getConfigParameter("p2.p21", numberValue, "defaultValue") == true);
	BOOST_REQUIRE(numberValue == 123);
	BOOST_REQUIRE(cfg1.getConfigParameter("p2.p21", stringValue, "defaultValue") == true);
	BOOST_REQUIRE(stringValue == "123");
	BOOST_REQUIRE(cfg1.getConfigParameter("p2.p21", numberValue, "1234") == true);
	BOOST_REQUIRE(numberValue == 123);
	BOOST_REQUIRE(cfg1.getConfigParameter("p2.p21", stringValue, "1234") == true);
	BOOST_REQUIRE(stringValue == "123");
	
	cfg2 = cfg1.getChild("p1");
	BOOST_REQUIRE(cfg2->getConfigParameter(stringValue) == true);
	BOOST_REQUIRE(stringValue == "v1");
	BOOST_REQUIRE(cfg2->getConfigParameter(numberValue) == false);
}

BOOST_AUTO_TEST_CASE(Config_value_array)
{
	Config cfg;

	BOOST_REQUIRE(cfg.setValue("p1", "v1") == true);
	BOOST_REQUIRE(cfg.setValue("p1", "v2") == false);

	BOOST_REQUIRE(cfg.addValue("p2", "v1") == true);
	BOOST_REQUIRE(cfg.addValue("p2", "v2") == true);

	BOOST_REQUIRE(cfg.addValue("p3.p1", "v1") == true);
	BOOST_REQUIRE(cfg.addValue("p3.p1", "v2") == true);

	BOOST_REQUIRE(cfg.addValue("p3.p2.p3", "v1") == true);
	BOOST_REQUIRE(cfg.addValue("p3.p2.p3", "v2") == true);

	std::vector<std::string> valueVec;

	valueVec.clear();
	cfg.getValues("p3", valueVec);
	BOOST_REQUIRE(valueVec.size() == 0);

	valueVec.clear();
	cfg.getValues("p2", valueVec);
	BOOST_REQUIRE(valueVec.size() == 2);
	BOOST_REQUIRE(valueVec[0] == "v1");
	BOOST_REQUIRE(valueVec[1] == "v2");

	valueVec.clear();
	cfg.getValues("p3.p1", valueVec);
	BOOST_REQUIRE(valueVec.size() == 2);
	BOOST_REQUIRE(valueVec[0] == "v1");
	BOOST_REQUIRE(valueVec[1] == "v2");

	valueVec.clear();
	cfg.getValues("p3.p2.p3", valueVec);
	BOOST_REQUIRE(valueVec.size() == 2);
	BOOST_REQUIRE(valueVec[0] == "v1");
	BOOST_REQUIRE(valueVec[1] == "v2");
}

BOOST_AUTO_TEST_CASE(Config_child_array)
{
	Config cfg1, cfg2;

	BOOST_REQUIRE(cfg1.setValue("p1", "v1") == true);
	BOOST_REQUIRE(cfg1.setValue("p1", "v2") == false);

	BOOST_REQUIRE(cfg1.addValue("p3.p1.p1", "v1") == true);
	BOOST_REQUIRE(cfg1.addValue("p3.p1.p2", "v2") == true);
	BOOST_REQUIRE(cfg1.addValue("p3.p1.p3", "v3") == true);
	BOOST_REQUIRE(cfg1.addValue("p3.p1.p4", "v4") == true);


	std::vector<Config> configVec;

	configVec.clear();
	cfg1.getChilds("p1", configVec);
	BOOST_REQUIRE(configVec.size() == 0);

	configVec.clear();
	cfg1.getChilds("p3", configVec);
	BOOST_REQUIRE(configVec.size() == 1);

	configVec.clear();
	cfg1.getChilds("p3.p1", configVec);
	BOOST_REQUIRE(configVec.size() == 1);

	BOOST_REQUIRE(cfg2.addValue("p1", "v11") == true);
	BOOST_REQUIRE(cfg2.addValue("p2", "v22") == true);
	BOOST_REQUIRE(cfg2.addValue("p3", "v33") == true);
	BOOST_REQUIRE(cfg2.addValue("p4", "v44") == true);

	BOOST_REQUIRE(cfg1.addChild("p3.p1", cfg2) == true);

	configVec.clear();
	cfg1.getChilds("p3.p1", configVec);
	BOOST_REQUIRE(configVec.size() == 2);

	BOOST_REQUIRE(*configVec[0].getValue("p1") == "v1");
	BOOST_REQUIRE(*configVec[0].getValue("p2") == "v2");
	BOOST_REQUIRE(*configVec[0].getValue("p3") == "v3");
	BOOST_REQUIRE(*configVec[0].getValue("p4") == "v4");
	BOOST_REQUIRE(*configVec[1].getValue("p1") == "v11");
	BOOST_REQUIRE(*configVec[1].getValue("p2") == "v22");
	BOOST_REQUIRE(*configVec[1].getValue("p3") == "v33");
	BOOST_REQUIRE(*configVec[1].getValue("p4") == "v44");

}

BOOST_AUTO_TEST_SUITE_END()
