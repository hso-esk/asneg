/*
    EventTypeClass: AuditSecurityEventType

    Generated Source Code - please do not change this source code

    EventTypeCodeGenerator Version:
        OpcUaStackCore - 3.0.1

    Autor: Kai Huebl (kai@huebl-sgh.de)
*/

#include "OpcUaStackCore/StandardEventType/AuditSecurityEventType.h"

namespace OpcUaStackCore
{
    
    AuditSecurityEventType::AuditSecurityEventType(void)
    : AuditEventType()
    , eventVariables_()
    {
        eventVariables_.registerEventVariable("EMPTY", OpcUaBuildInType_OpcUaVariant);
    
        eventVariables_.eventType(OpcUaNodeId((OpcUaUInt32)2058));
        eventVariables_.namespaceIndex(0);
        eventVariables_.browseName(OpcUaQualifiedName("AuditSecurityEventType"));
        eventVariables_.namespaceUri("");
    }
    
    AuditSecurityEventType::~AuditSecurityEventType(void)
    {
    }
    
    OpcUaVariant::SPtr 
    AuditSecurityEventType::getAuditSecurityEventType(void)
    {
    	OpcUaVariant::SPtr value;
    	eventVariables_.getValue("EMPTY", value);
    	return value;
    }
    
    bool 
    AuditSecurityEventType::setAuditSecurityEventType(OpcUaVariant::SPtr& value)
    {
    	return eventVariables_.setValue("EMPTY", value);
    }
    
    void
    AuditSecurityEventType::mapNamespaceUri(void)
    {
        uint32_t namespaceIndex;
        AuditEventType::mapNamespaceUri();
    
        OpcUaVariant::SPtr eventTypeVariable = constructSPtr<OpcUaVariant>();
        eventTypeVariable->setValue(eventVariables_.eventType());
    
        setNamespaceIndex(eventVariables_.namespaceUri(), namespaceIndex, eventVariables_.browseName(), eventTypeVariable);
    
        eventType(eventTypeVariable);
        eventVariables_.eventType(eventTypeVariable);
        eventVariables_.namespaceIndex(namespaceIndex);
    }

    OpcUaVariant::SPtr
    AuditSecurityEventType::get(
    	OpcUaNodeId& eventType,
    	std::list<OpcUaQualifiedName::SPtr>& browseNameList,
    	EventResult::Code& resultCode
    )
    {
        resultCode = EventResult::Success;

        // check whether eventType and typeNodeId are identical
        if (eventType == eventVariables_.eventType()) {
    	    return eventVariables_.get(browseNameList, resultCode);
        }

        // the start item was not found. We delegate the search to the base class
        OpcUaVariant::SPtr variant;
        variant = AuditEventType::get(eventType, browseNameList, resultCode);
        if (resultCode != EventResult::Success || browseNameList.empty()) {
    	    return variant;
        }

        return eventVariables_.get(browseNameList, resultCode);
    }

}
