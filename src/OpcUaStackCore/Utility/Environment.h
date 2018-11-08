/*
   Copyright 2015-2018 Kai Huebl (kai@huebl-sgh.de)

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

#ifndef __OpcUaStackCore_Environment_h__
#define __OpcUaStackCore_Environment_h__

#include <boost/asio.hpp>
#include "OpcUaStackCore/Base/os.h"
#include <string>

namespace OpcUaStackCore
{

	class DLLEXPORT Environment
	{
	  public:
		Environment(void);
		~Environment(void);

		static std::string confDir_;

		static std::string hostname(void);
		static std::vector<boost::asio::ip::address> ips(void);

		static std::string getApplicationPathAbsolute(void);
		static std::string getInstallationPathRelative(const std::string& binaryDirectory);
		static std::string getInstallationPathAbsolute(
			const std::string& serviceName,
			const std::string& configFileName,
			const std::string& confDirectory
		);
		static std::string getCurrentPath(void);
		static void setCurrentPath(const std::string& currentPath);
		static std::string getAbsolutePath(const std::string& relativePath);

		static void confDir(const std::string& confDir);
		static std::string& confDir(void);

		static std::string subst(const std::string& string);
	};

}

#endif
