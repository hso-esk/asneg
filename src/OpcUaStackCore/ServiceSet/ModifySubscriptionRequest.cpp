#include "OpcUaStackCore/ServiceSet/ModifySubscriptionRequest.h"

namespace OpcUaStackCore
{

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// OpcUa ModifySubscriptionRequest
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------

	ModifySubscriptionRequest::ModifySubscriptionRequest(void)
	: ObjectPool<ModifySubscriptionRequest>()
	, requestHeaderSPtr_(RequestHeader::construct())
	, subscriptionId_()
	, requestedPublishingInterval_()
	, requestedLifetimeCount_()
	, requestedMaxKeepAliveCount_()
	, maxNotificationsPerPublish_()
	, priority_()
	{
	}

	ModifySubscriptionRequest::~ModifySubscriptionRequest(void)
	{
	}

	void 
	ModifySubscriptionRequest::requestHeader(const RequestHeader::SPtr requestHeader)
	{
		requestHeaderSPtr_ = requestHeader;
	}

	RequestHeader::SPtr 
	ModifySubscriptionRequest::requestHeader(void) const
	{
		return requestHeaderSPtr_;
	}

	void 
	ModifySubscriptionRequest::subscriptionId(const OpcUaUInt32& subscriptionId)
	{
		subscriptionId_ = subscriptionId;
	}
	
	OpcUaUInt32 
	ModifySubscriptionRequest::subscriptionId(void) const
	{
		return subscriptionId_;
	}

	void 
	ModifySubscriptionRequest::requestedPublishingInterval(const OpcUaDouble& requestedPublishingInterval)
	{
		requestedPublishingInterval_ = requestedPublishingInterval;
	}
	
	OpcUaDouble 
	ModifySubscriptionRequest::requestedPublishingInterval(void) const
	{
		return requestedPublishingInterval_;
	}

	void 
	ModifySubscriptionRequest::requestedLifetimeCount(const OpcUaUInt32& requestedLifetimeCount)
	{
		requestedLifetimeCount_ = requestedLifetimeCount;
	}

	OpcUaUInt32 
	ModifySubscriptionRequest::requestedLifetimeCount(void) const
	{
		return requestedLifetimeCount_;
	}

	void 
	ModifySubscriptionRequest::requestedMaxKeepAliveCount(const OpcUaUInt32& requestedMaxKeepAliveCount)
	{
		requestedMaxKeepAliveCount_ = requestedMaxKeepAliveCount;
	}

	OpcUaUInt32 
	ModifySubscriptionRequest::requestedMaxKeepAliveCount(void) const
	{
		return requestedMaxKeepAliveCount_;
	}

	void 
	ModifySubscriptionRequest::maxNotificationsPerPublish(const OpcUaUInt32& maxNotificationsPerPublish)
	{
		maxNotificationsPerPublish_ = maxNotificationsPerPublish;
	}
	
	OpcUaUInt32 
	ModifySubscriptionRequest::maxNotificationsPerPublish(void) const 
	{
		return maxNotificationsPerPublish_;
	}
	
	void 
	ModifySubscriptionRequest::priority(const OpcUaByte& priority)
	{
		priority_ = priority;
	}

	OpcUaByte 
	ModifySubscriptionRequest::priority(void) const
	{
		return priority_;
	}
	
	void 
	ModifySubscriptionRequest::opcUaBinaryEncode(std::ostream& os) const
	{
		requestHeaderSPtr_->opcUaBinaryEncode(os);
		OpcUaNumber::opcUaBinaryEncode(os, subscriptionId_);
		OpcUaNumber::opcUaBinaryEncode(os, requestedPublishingInterval_);
		OpcUaNumber::opcUaBinaryEncode(os, requestedLifetimeCount_);
		OpcUaNumber::opcUaBinaryEncode(os, requestedMaxKeepAliveCount_);
		OpcUaNumber::opcUaBinaryEncode(os, maxNotificationsPerPublish_);
		OpcUaNumber::opcUaBinaryEncode(os, priority_);
	}
	
	void 
	ModifySubscriptionRequest::opcUaBinaryDecode(std::istream& is)
	{
		requestHeaderSPtr_->opcUaBinaryDecode(is);
		OpcUaNumber::opcUaBinaryDecode(is, subscriptionId_);
		OpcUaNumber::opcUaBinaryDecode(is, requestedPublishingInterval_);
		OpcUaNumber::opcUaBinaryDecode(is, requestedLifetimeCount_);
		OpcUaNumber::opcUaBinaryDecode(is, requestedMaxKeepAliveCount_);
		OpcUaNumber::opcUaBinaryDecode(is, maxNotificationsPerPublish_);
		OpcUaNumber::opcUaBinaryDecode(is, priority_);
	}
}