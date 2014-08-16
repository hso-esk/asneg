#ifndef __OpcUaStackCore_WriteRequest_h__
#define __OpcUaStackCore_WriteRequest_h__

#include <stdint.h>
#include "OpcUaStackCore/Base/ObjectPool.h"
#include "OpcUaStackCore/Base/os.h"
#include "OpcUaStackCore/BuildInTypes/BuildInTypes.h"
#include "OpcUaStackCore/SecureChannel/RequestHeader.h"
#include "OpcUaStackCore/ServiceSet/WriteValue.h"

namespace OpcUaStackCore
{

	class DLLEXPORT WriteRequest : public  ObjectPool<WriteRequest>
	{
	  public:
		WriteRequest(void);
		virtual ~WriteRequest(void);

		void requestHeader(const RequestHeader::SPtr requestHeader);
		RequestHeader::SPtr requestHeader(void) const;
		void writeValueArray(const WriteValueArray::SPtr writeValueArray);
		WriteValueArray::SPtr writeValueArray(void) const;

		void opcUaBinaryEncode(std::ostream& os) const;
		void opcUaBinaryDecode(std::istream& is);

	  private:
		  RequestHeader::SPtr requestHeaderSPtr_;
		  WriteValueArray::SPtr writeValueArraySPtr_;

	};

}

#endif