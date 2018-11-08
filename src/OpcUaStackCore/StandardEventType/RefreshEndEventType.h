/*
    EventTypeClass: RefreshEndEventType

    Generated Source Code - please do not change this source code

    EventTypeCodeGenerator Version:
        OpcUaStackCore - 3.0.1

    Autor: Kai Huebl (kai@huebl-sgh.de)
*/

#ifndef __OpcUaStackCore_RefreshEndEventType_h__
#define __OpcUaStackCore_RefreshEndEventType_h__

#include <boost/shared_ptr.hpp>
#include "OpcUaStackCore/Base/os.h"
#include "OpcUaStackCore/BuildInTypes/BuildInTypes.h"
#include "OpcUaStackCore/StandardEventType/SystemEventType.h"

namespace OpcUaStackCore
{
    
    class DLLEXPORT RefreshEndEventType
    : public SystemEventType
    {
      public:
        typedef boost::shared_ptr<RefreshEndEventType> SPtr;
    
        RefreshEndEventType(void);
        virtual ~RefreshEndEventType(void);
        bool setRefreshEndEventType(OpcUaVariant::SPtr& variable);
        OpcUaVariant::SPtr getRefreshEndEventType(void);
        
        
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
