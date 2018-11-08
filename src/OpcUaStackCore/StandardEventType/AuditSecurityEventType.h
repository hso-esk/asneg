/*
    EventTypeClass: AuditSecurityEventType

    Generated Source Code - please do not change this source code

    EventTypeCodeGenerator Version:
        OpcUaStackCore - 3.0.1

    Autor: Kai Huebl (kai@huebl-sgh.de)
*/

#ifndef __OpcUaStackCore_AuditSecurityEventType_h__
#define __OpcUaStackCore_AuditSecurityEventType_h__

#include <boost/shared_ptr.hpp>
#include "OpcUaStackCore/Base/os.h"
#include "OpcUaStackCore/BuildInTypes/BuildInTypes.h"
#include "OpcUaStackCore/StandardEventType/AuditEventType.h"

namespace OpcUaStackCore
{
    
    class DLLEXPORT AuditSecurityEventType
    : public AuditEventType
    {
      public:
        typedef boost::shared_ptr<AuditSecurityEventType> SPtr;
    
        AuditSecurityEventType(void);
        virtual ~AuditSecurityEventType(void);
        bool setAuditSecurityEventType(OpcUaVariant::SPtr& variable);
        OpcUaVariant::SPtr getAuditSecurityEventType(void);
        
        
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
