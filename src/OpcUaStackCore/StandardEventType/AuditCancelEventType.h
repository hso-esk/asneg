/*
    EventTypeClass: AuditCancelEventType

    Generated Source Code - please do not change this source code

    EventTypeCodeGenerator Version:
        OpcUaStackCore - 3.0.1

    Autor: Kai Huebl (kai@huebl-sgh.de)
*/

#ifndef __OpcUaStackCore_AuditCancelEventType_h__
#define __OpcUaStackCore_AuditCancelEventType_h__

#include <boost/shared_ptr.hpp>
#include "OpcUaStackCore/Base/os.h"
#include "OpcUaStackCore/BuildInTypes/BuildInTypes.h"
#include "OpcUaStackCore/StandardEventType/AuditSessionEventType.h"

namespace OpcUaStackCore
{
    
    class DLLEXPORT AuditCancelEventType
    : public AuditSessionEventType
    {
      public:
        typedef boost::shared_ptr<AuditCancelEventType> SPtr;
    
        AuditCancelEventType(void);
        virtual ~AuditCancelEventType(void);
        bool requestHandle(OpcUaVariant::SPtr& variable);
        OpcUaVariant::SPtr requestHandle(void);
        
        bool setAuditCancelEventType(OpcUaVariant::SPtr& variable);
        OpcUaVariant::SPtr getAuditCancelEventType(void);
        
        
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
