/*
    EventTypeClass: AuditConditionEnableEventType

    Generated Source Code - please do not change this source code

    EventTypeCodeGenerator Version:
        OpcUaStackCore - 3.0.1

    Autor: Kai Huebl (kai@huebl-sgh.de)
*/

#ifndef __OpcUaStackCore_AuditConditionEnableEventType_h__
#define __OpcUaStackCore_AuditConditionEnableEventType_h__

#include <boost/shared_ptr.hpp>
#include "OpcUaStackCore/Base/os.h"
#include "OpcUaStackCore/BuildInTypes/BuildInTypes.h"
#include "OpcUaStackCore/StandardEventType/AuditConditionEventType.h"

namespace OpcUaStackCore
{
    
    class DLLEXPORT AuditConditionEnableEventType
    : public AuditConditionEventType
    {
      public:
        typedef boost::shared_ptr<AuditConditionEnableEventType> SPtr;
    
        AuditConditionEnableEventType(void);
        virtual ~AuditConditionEnableEventType(void);
        bool setAuditConditionEnableEventType(OpcUaVariant::SPtr& variable);
        OpcUaVariant::SPtr getAuditConditionEnableEventType(void);
        
        
        //- EventBase interface
        virtual void mapNamespaceUri(void);
        
        virtual OpcUaVariant::SPtr get(
            OpcUaNodeId& eventType,
            std::list<OpcUaQualifiedName::SPtr>& browseNameList,
            EventResult::Code& resultCode
        );
        //- EventBase interface
        
    
      private:
        EventVariables eventVariables_;
    
    };

}

#endif
