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

#ifndef __OpcUaStackCore_AcknowledgeMessage_h__
#define __OpcUaStackCore_AcknowledgeMessage_h__

#include "OpcUaStackCore/Base/ObjectPool.h"
#include "OpcUaStackCore/Base/os.h"
#include "OpcUaStackCore/BuildInTypes/OpcUaNumber.h"

namespace OpcUaStackCore
{

	class DLLEXPORT AcknowledgeMessage : public  ObjectPool<AcknowledgeMessage>
	{
	  public:
		typedef boost::shared_ptr<AcknowledgeMessage> SPtr;

		AcknowledgeMessage(void);
		virtual ~AcknowledgeMessage(void);

		void protocolVersion(const OpcUaUInt32& protocolVersion);
		OpcUaUInt32 protocolVersion(void) const;
		void receivedBufferSize(const OpcUaUInt32& receivedBufferSize);
		OpcUaUInt32 receivedBufferSize(void) const;
		void sendBufferSize(const OpcUaUInt32& sendBufferSize);
		OpcUaUInt32 sendBufferSize(void) const;
		void maxMessageSize(const OpcUaUInt32& maxMessageSize);
		OpcUaUInt32 maxMessageSize(void) const;
		void maxChunkCount(const OpcUaUInt32& maxChunkCount);
		OpcUaUInt32 maxChunkCount(void) const;

		void opcUaBinaryEncode(std::ostream& os) const;
		void opcUaBinaryDecode(std::istream& is);

	  private:
		OpcUaUInt32 protocolVersion_;
		OpcUaUInt32 receivedBufferSize_;
		OpcUaUInt32 sendBufferSize_;
		OpcUaUInt32 maxMessageSize_;
		OpcUaUInt32 maxChunkCount_;
	};

}

#endif