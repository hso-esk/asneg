#include "unittest.h"
#include <iostream>
#include "OpcUaStackCore/Base/Address.h"

using namespace OpcUaStackCore;

BOOST_AUTO_TEST_SUITE(Address_t)

BOOST_AUTO_TEST_CASE(Address_)
{
	std::cout << "Address_t" << std::endl;
}

BOOST_AUTO_TEST_CASE(Address_HostName)
{
	std::cout << "HostName=" << Address::hostName() << std::endl;
}

BOOST_AUTO_TEST_CASE(Address_decode)
{
	std::vector<std::string>::iterator it;
	std::vector<std::string> ipVec;
	Address::getAllIPv4sFromHost(ipVec);
	for (it=ipVec.begin(); it!=ipVec.end(); it++) {
		std::cout << "IP=" << *it << std::endl;
	}
}

BOOST_AUTO_TEST_CASE(Address_getIpsFromHost)
{
	std::cout << "getIpsFromHost" << std::endl;

	std::string hostname = Address::hostName();
	std::vector<std::string> ipVec;
	Address::getIpsFromHostname(hostname.c_str(), ipVec);

	std::vector<std::string>::iterator it;
	for (it=ipVec.begin(); it!=ipVec.end(); it++) {
		std::cout << "IP=" << *it << std::endl;
	}
}

BOOST_AUTO_TEST_CASE(Address_getHostFromIp)
{
	std::cout << "getHostnameFromIp" << std::endl;

	std::vector<std::string>::iterator it;
	std::vector<std::string> ipVec;
	Address::getAllIPv4sFromHost(ipVec);
	for (it=ipVec.begin(); it!=ipVec.end(); it++) {
		std::string ip = *it;
		std::string hostname;

		Address::getHostnameFromIp(ip, hostname);
		std::cout << ip << " " << hostname << std::endl;
	}
}

BOOST_AUTO_TEST_SUITE_END()
