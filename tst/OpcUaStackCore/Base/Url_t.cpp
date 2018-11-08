#include "unittest.h"
#include <iostream>
#include "OpcUaStackCore/Base/Url.h"

using namespace OpcUaStackCore;

BOOST_AUTO_TEST_SUITE(Url_t)

BOOST_AUTO_TEST_CASE(Url_)
{
	std::cout << "Url_t" << std::endl;
}

BOOST_AUTO_TEST_CASE(Url_parse)
{
	Url url;

	url.url("opc.tcp://localhost:4841/path/to?query");
	BOOST_REQUIRE(url.protocol() == "opc.tcp");
	BOOST_REQUIRE(url.host() == "localhost");
	BOOST_REQUIRE(url.port() == 4841);
	BOOST_REQUIRE(url.path() == "path/to");
	BOOST_REQUIRE(url.query() == "query");

	url.url("opc.tcp://localhost:4841/path/to");
	BOOST_REQUIRE(url.protocol() == "opc.tcp");
	BOOST_REQUIRE(url.host() == "localhost");
	BOOST_REQUIRE(url.port() == 4841);
	BOOST_REQUIRE(url.path() == "path/to");
	BOOST_REQUIRE(url.query() == "");

	url.url("opc.tcp://localhost:4841");
	BOOST_REQUIRE(url.protocol() == "opc.tcp");
	BOOST_REQUIRE(url.host() == "localhost");
	BOOST_REQUIRE(url.port() == 4841);
	BOOST_REQUIRE(url.path() == "");
	BOOST_REQUIRE(url.query() == "");

	url.url("opc.tcp://localhost");
	BOOST_REQUIRE(url.protocol() == "opc.tcp");
	BOOST_REQUIRE(url.host() == "localhost");
	BOOST_REQUIRE(url.port() == -1);
	BOOST_REQUIRE(url.path() == "");
	BOOST_REQUIRE(url.query() == "");

	url.url("localhost");
	BOOST_REQUIRE(url.protocol() == "");
	BOOST_REQUIRE(url.host() == "localhost");
	BOOST_REQUIRE(url.port() == -1);
	BOOST_REQUIRE(url.path() == "");
	BOOST_REQUIRE(url.query() == "");



	url.url("localhost:4841/path/to?query");
	BOOST_REQUIRE(url.protocol() == "");
	BOOST_REQUIRE(url.host() == "localhost");
	BOOST_REQUIRE(url.port() == 4841);
	BOOST_REQUIRE(url.path() == "path/to");
	BOOST_REQUIRE(url.query() == "query");

	url.url("opc.tcp://localhost/path/to?query");
	BOOST_REQUIRE(url.protocol() == "opc.tcp");
	BOOST_REQUIRE(url.host() == "localhost");
	BOOST_REQUIRE(url.port() == -1);
	BOOST_REQUIRE(url.path() == "path/to");
	BOOST_REQUIRE(url.query() == "query");

	url.url("opc.tcp://localhost:4841?query");
	BOOST_REQUIRE(url.protocol() == "opc.tcp");
	BOOST_REQUIRE(url.host() == "localhost");
	BOOST_REQUIRE(url.port() == 4841);
	BOOST_REQUIRE(url.path() == "");
	BOOST_REQUIRE(url.query() == "query");

	url.url("opc.tcp://localhost:4841/path/to");
	BOOST_REQUIRE(url.protocol() == "opc.tcp");
	BOOST_REQUIRE(url.host() == "localhost");
	BOOST_REQUIRE(url.port() == 4841);
	BOOST_REQUIRE(url.path() == "path/to");
	BOOST_REQUIRE(url.query() == "");
}

BOOST_AUTO_TEST_CASE(Url_check_function)
{
	Url url;

	url.url("opc.tcp://localhost:4841/path/to?query");
	BOOST_REQUIRE(url.isLocalAddress() == true);
	BOOST_REQUIRE(url.isAnyAddress() == false);
	BOOST_REQUIRE(url.isIPAddress() == false);
	BOOST_REQUIRE(url.isHostAddress() == false);

	url.url("opc.tcp://Localhost:4841/path/to?query");
	BOOST_REQUIRE(url.isLocalAddress() == true);
	BOOST_REQUIRE(url.isAnyAddress() == false);
	BOOST_REQUIRE(url.isIPAddress() == false);
	BOOST_REQUIRE(url.isHostAddress() == false);

	url.url("opc.tcp://127.0.0.1:4841/path/to?query");
	BOOST_REQUIRE(url.isLocalAddress() == true);
	BOOST_REQUIRE(url.isAnyAddress() == false);
	BOOST_REQUIRE(url.isIPAddress() == true);
	BOOST_REQUIRE(url.isHostAddress() == false);

	url.url("opc.tcp://127.000.000.001:4841/path/to?query");
	BOOST_REQUIRE(url.isLocalAddress() == true);
	BOOST_REQUIRE(url.isAnyAddress() == false);
	BOOST_REQUIRE(url.isIPAddress() == true);
	BOOST_REQUIRE(url.isHostAddress() == false);

	url.url("opc.tcp://127.001.001.001:4841/path/to?query");
	BOOST_REQUIRE(url.isLocalAddress() == false);
	BOOST_REQUIRE(url.isAnyAddress() == false);
	BOOST_REQUIRE(url.isIPAddress() == true);
	BOOST_REQUIRE(url.isHostAddress() == false);

	url.url("opc.tcp://127.100.100.100:4841/path/to?query");
	BOOST_REQUIRE(url.isLocalAddress() == false);
	BOOST_REQUIRE(url.isAnyAddress() == false);
	BOOST_REQUIRE(url.isIPAddress() == true);
	BOOST_REQUIRE(url.isHostAddress() == false);

	url.url("opc.tcp://ASNeG.de:4841/path/to?query");
	BOOST_REQUIRE(url.isLocalAddress() == false);
	BOOST_REQUIRE(url.isAnyAddress() == false);
	BOOST_REQUIRE(url.isIPAddress() == false);
	BOOST_REQUIRE(url.isHostAddress() == true);

	url.url("opc.tcp://0.0.0.0:4841/path/to?query");
	BOOST_REQUIRE(url.isLocalAddress() == false);
	BOOST_REQUIRE(url.isAnyAddress() == true);
	BOOST_REQUIRE(url.isIPAddress() ==  true);
	BOOST_REQUIRE(url.isHostAddress() == false);
}

BOOST_AUTO_TEST_SUITE_END()
