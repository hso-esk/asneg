/*
    EventTypeClass: AuditUpdateMethodEventType

    Generated Source Code - please do not change this source code

    EventTypeCodeGenerator Version:
        OpcUaStackCore - 3.0.1

    Autor: Kai Huebl (kai@huebl-sgh.de)
*/

#ifndef __OpcUaStackCore_AuditUpdateMethodEventType_h__
#define __OpcUaStackCore_AuditUpdateMethodEventType_h__

#include <boost/shared_ptr.hpp>
#include "OpcUaStackCore/Base/os.h"
#include "OpcUaStackCore/BuildInTypes/BuildInTypes.h"
#include "OpcUaStackCore/StandardEventType/AuditEventType.h"

namespace OpcUaStackCore
{
    
    class DLLEXPORT AuditUpdateMethodEventType
    : public AuditEventType
    {
      public:
        typedef boost::shared_ptr<AuditUpdateMethodEventType> SPtr;
    
        AuditUpdateMethodEventType(void);
        virtual ~AuditUpdateMethodEventType(void);
        bool methodId(OpcUaVariant::SPtr& variable);
        OpcUaVariant::SPtr methodId(void);
        
        bool inputArguments(OpcUaVariant::SPtr& variable);
        OpcUaVariant::SPtr inputArguments(void);
        
        bool setAuditUpdateMethodEventType(OpcUaVariant::SPtr& variable);
        OpcUaVariant::SPtr getAuditUpdateMethodEventType(void);
        
        
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
