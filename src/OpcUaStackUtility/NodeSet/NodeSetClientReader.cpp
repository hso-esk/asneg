#include "OpcUaStackUtility/NodeSet/NodeSetClientReader.h"
#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaStackClient/ServiceSet/AttributeService.h"
#include "OpcUaStackClient/ServiceSet/ViewService.h"

using namespace OpcUaStackClient;

namespace OpcUaStackUtility
{

	NodeSetClientReader::NodeSetClientReader(void)
	: client_()
	, nodeSetNamespace_()
	, condition_()
	, error_(false)
	{
		client_.init();
		client_.start();
	}

	NodeSetClientReader::~NodeSetClientReader(void)
	{
		client_.stop();
		client_.cleanup();
	}

	bool 
	NodeSetClientReader::readNodes(
		const std::string& sessionConfigPrefix, Config& sessionConfig,
		const std::string& secureChannelConfigPrefix, Config& secureChannelConfig,
		uint32_t operationTimeout
	)
	{
		bool rc;

		operationTimeout_ = operationTimeout;

		// open session to opc ua server. A condition is used to wait for
		// the end of the operation
		condition_.conditionInit();
		session_ = client_.sessionManager().getNewSession(
			sessionConfigPrefix, sessionConfig,
			secureChannelConfigPrefix, secureChannelConfig,
			this
		);
		session_->createSession();
		rc = condition_.waitForCondition(operationTimeout_);
		if (!rc) {
			Log(Error, "create session timeout in node set client reader");
			return false;
		}
		if (error_) {
			session_->closeSession();
			Log(Error, "create Session error");
			return false;
		}

		// activate session on the opc ua server. A condition is used to wait
		// for the end of the operation
		condition_.conditionInit();
		session_->activateSession();
		rc = condition_.waitForCondition(operationTimeout_);
		if (!rc) {
			Log(Error, "activate session timeout in node set client reader");
			return false;
		}
		if (error_) {
			session_->closeSession();
			Log(Error, "activate Session error");
			return false;
		}

		// read namespace array from opc ua server
		if (!readNamespaceArray()) {
			session_->closeSession();
			Log(Error, "read namespace array error");
			return false;
		}

		return true;
	}

	NodeSetNamespace& 
	NodeSetClientReader::nodeSetNamespace(void)
	{
		return nodeSetNamespace_;
	}

	bool 
	NodeSetClientReader::readNamespaceArray(void)
	{
		AttributeService attributeService;
		attributeService.componentSession(session_->component());

		//
		// read namespace array
		//
		ServiceTransactionRead::SPtr readTrx = ServiceTransactionRead::construct();
		
		ReadValueId::SPtr readValueIdSPtr = ReadValueId::construct();
		readValueIdSPtr->nodeId((OpcUaInt16)0, (OpcUaInt32)2255);
		readValueIdSPtr->attributeId((OpcUaInt32) 13);
		readValueIdSPtr->dataEncoding().namespaceIndex((OpcUaInt16) 0);

		ReadRequest::SPtr req = readTrx->request();
		req->maxAge(0);
		req->timestampsToReturn(2);
		req->readValueIdArray()->set(readValueIdSPtr);

		attributeService.sendSync(readTrx);

		//
		// check response
		//
		if (readTrx->responseHeader()->serviceResult() != Success) {
			Log(Error, "read namespace array response error")
				.parameter("NodeId", readValueIdSPtr->nodeId())
				.parameter("AttributeId", readValueIdSPtr->attributeId())
				.parameter("ResultCode", OpcUaStatusCodeMap::longString(readTrx->responseHeader()->serviceResult()));
			return false;
		}
		if (readTrx->response()->dataValueArray()->size() != 1) {
			Log(Error, "read namespace array array size error")
				.parameter("NodeId", readValueIdSPtr->nodeId())
				.parameter("AttributeId", readValueIdSPtr->attributeId())
				.parameter("ArraySize", readTrx->response()->dataValueArray()->size());
			return false;
		}

		//
		// check data value
		//
		OpcUaDataValue::SPtr dataValue;
		readTrx->response()->dataValueArray()->get(0, dataValue);
		OpcUaVariant::SPtr variant = dataValue->variant();
		if (dataValue->statusCode() != Success) {
			Log(Error, "read namespace array data value error")
				.parameter("NodeId", readValueIdSPtr->nodeId())
				.parameter("AttributeId", readValueIdSPtr->attributeId())
				.parameter("StatusCode", OpcUaStatusCodeMap::longString(dataValue->statusCode()));
		}

		if (variant->arrayLength() == 0) {
			Log(Error, "read namespace array data value array length error")
				.parameter("NodeId", readValueIdSPtr->nodeId())
				.parameter("AttributeId", readValueIdSPtr->attributeId())
				.parameter("ValueArrayLen", variant->arrayLength());
		}

		//
		// add namespace array from data value into node set namespace array 
		//
		std::vector<std::string> namespaceUriVec;
		for (int32_t idx=0; idx < variant->arrayLength(); idx++) {
			namespaceUriVec.push_back(variant->variant()[idx].variantSPtr<OpcUaString>()->value());
		}
		nodeSetNamespace_.clear();
		nodeSetNamespace_.decodeNamespaceUris(namespaceUriVec);

		//
		// browse nodes from opc ua server
		//
		nodeIdMap_.clear();
		OpcUaNodeId nodeId;
		nodeId.set(84);
		if (!browse(nodeId)) {
			return false;
		}

		std::cout << "Nodes=" << nodeIdMap_.size() << std::endl;
		return true;
	}

	bool 
	NodeSetClientReader::browse(OpcUaNodeId& nodeId)
	{
		ViewService viewService;
		viewService.componentSession(session_->component());

		//
		// browse node
		//
		ServiceTransactionBrowse::SPtr browseTrx = ServiceTransactionBrowse::construct();
		BrowseRequest::SPtr req = browseTrx->request();

		BrowseDescription::SPtr browseDescription = BrowseDescription::construct();
		nodeId.copyTo(*browseDescription->nodeId());
		
		BrowseDescriptionArray::SPtr browseDescriptionArray = BrowseDescriptionArray::construct();
		req->nodesToBrowse()->resize(1);
		req->nodesToBrowse()->set(browseDescription);

		viewService.sendSync(browseTrx);

		//
		// check response
		//
		if (browseTrx->responseHeader()->serviceResult() != Success) {
			Log(Error, "browse node response error")
				.parameter("NodeId", nodeId)
				.parameter("ResultCode", OpcUaStatusCodeMap::longString(browseTrx->responseHeader()->serviceResult()));
			return false;
		}
		if (browseTrx->response()->results()->size() != 1) {
			Log(Error, "browse node array size error")
				.parameter("NodeId", nodeId)
				.parameter("ArraySize", browseTrx->response()->results()->size());
			return false;
		}

		//
		// check dataValue
		// 
		BrowseResult::SPtr browseResult;
		browseTrx->response()->results()->get(0, browseResult);
		if (browseResult->statusCode() != Success) {
			Log(Error, "browse node result error")
				.parameter("NodeId", nodeId)
				.parameter("StatusCode", OpcUaStatusCodeMap::longString(browseResult->statusCode()));
		}

		//
		// read node information from browse result
		//
		ReferenceDescriptionArray::SPtr references = browseResult->references();
		for (uint32_t idx=0; idx<references->size(); idx++) {
			ReferenceDescription::SPtr referenceDescription;
			references->get(idx, referenceDescription);

			// check if node already exist
			OpcUaNodeId nodeId;
			nodeId.nodeIdValue(referenceDescription->nodeId()->nodeIdValue());
			nodeId.namespaceIndex(referenceDescription->nodeId()->namespaceIndex());

			if (nodeIdMap_.find(nodeId) != nodeIdMap_.end()) {
				continue;
			}
			nodeIdMap_.insert(std::make_pair(nodeId, referenceDescription));

			// read node
			if (!browse(nodeId)) return false;
		}

		return true;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// SessionIf Interface
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	void 
	NodeSetClientReader::error(void)
	{
		Log(Error, "session error in node set client reader");
		error_ = true;
		condition_.conditionTrue();
	}

	void 
	NodeSetClientReader::createSessionComplete(OpcUaStatusCode opcUaStatusCode)
	{
		Log(Debug, "create session complete in node set client reader")
			.parameter("StatusCode", opcUaStatusCode);
		if (opcUaStatusCode != Success) error_ = true;
		condition_.conditionTrue();
	}

	void 
	NodeSetClientReader::activateSessionComplete(OpcUaStatusCode opcUaStatusCode)
	{
		Log(Debug, "activate session complete in node set client reader")
			.parameter("StatusCode", opcUaStatusCode);
		if (opcUaStatusCode != Success) error_ = true;
		condition_.conditionTrue();
	}

}