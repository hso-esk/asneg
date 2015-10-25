/*
   Copyright 2015 Kai Huebl (kai@huebl-sgh.de)

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

#ifndef __OpcUaStackCore_ServerStatusDataType_h__
#define __OpcUaStackCore_ServerStatusDataType_h__

#include "OpcUaStackCore/Base/os.h"
#include "OpcUaStackCore/BuildInTypes/OpcUaLocalizedText.h"
#include "OpcUaStackCore/BuildInTypes/OpcUaDateTime.h"
#include "OpcUaStackCore/StandardDataTypes/BuildInfo.h"

namespace OpcUaStackCore
{

	class DLLEXPORT ServerStatusDataType
	: public ObjectPool<ServerStatusDataType>
	, public ExtensionObjectBase
	{
	  public:
		typedef boost::shared_ptr<ServerStatusDataType> SPtr;

		ServerStatusDataType(void);
		virtual ~ServerStatusDataType(void);

		OpcUaDateTime& startTime(void);
		OpcUaDateTime& currentTime(void);
		OpcUaInt32& serverState(void);
		BuildInfo& buildInfo(void);
		OpcUaUInt32& secondsTillShutdown(void);
		OpcUaLocalizedText& shutdownReason(void);

		void copyTo(ServerStatusDataType& serverStatusDataType);
		bool operator==(const ServerStatusDataType& serverStatusDataType) const;

		//- ExtensionObjectBase -----------------------------------------------
		ExtensionObjectBase::BSPtr factory(void);
		void opcUaBinaryEncode(std::ostream& os) const;
		void opcUaBinaryDecode(std::istream& is);
		void copyTo(ExtensionObjectBase& extensionObjectBase);
		bool equal(ExtensionObjectBase& extensionObjectBase) const;
		void out(std::ostream& os);
		//- ExtensionObjectBase -----------------------------------------------

	  private:
		OpcUaDateTime startTime_;
		OpcUaDateTime currentTime_;
		OpcUaInt32 serverState_;
		BuildInfo buildInfo_;
		OpcUaUInt32 secondsTillShutdown_;
		OpcUaLocalizedText shutdownReason_;
	};

}

#endif
