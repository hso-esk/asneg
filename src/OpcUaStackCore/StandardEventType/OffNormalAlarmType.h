/*
    EventTypeClass: OffNormalAlarmType

    Generated Source Code - please do not change this source code

    EventTypeCodeGenerator Version:
        OpcUaStackCore - 3.0.1

    Autor: Kai Huebl (kai@huebl-sgh.de)
*/

#ifndef __OpcUaStackCore_OffNormalAlarmType_h__
#define __OpcUaStackCore_OffNormalAlarmType_h__

#include <boost/shared_ptr.hpp>
#include "OpcUaStackCore/Base/os.h"
#include "OpcUaStackCore/BuildInTypes/BuildInTypes.h"
#include "OpcUaStackCore/StandardEventType/DiscreteAlarmType.h"

namespace OpcUaStackCore
{
    
    class DLLEXPORT OffNormalAlarmType
    : public DiscreteAlarmType
    {
      public:
        typedef boost::shared_ptr<OffNormalAlarmType> SPtr;
    
        OffNormalAlarmType(void);
        virtual ~OffNormalAlarmType(void);
        bool normalState(OpcUaVariant::SPtr& variable);
        OpcUaVariant::SPtr normalState(void);
        
        bool setOffNormalAlarmType(OpcUaVariant::SPtr& variable);
        OpcUaVariant::SPtr getOffNormalAlarmType(void);
        
        
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
