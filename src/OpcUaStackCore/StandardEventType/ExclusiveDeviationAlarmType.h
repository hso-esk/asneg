/*
    EventTypeClass: ExclusiveDeviationAlarmType

    Generated Source Code - please do not change this source code

    EventTypeCodeGenerator Version:
        OpcUaStackCore - 3.0.1

    Autor: Kai Huebl (kai@huebl-sgh.de)
*/

#ifndef __OpcUaStackCore_ExclusiveDeviationAlarmType_h__
#define __OpcUaStackCore_ExclusiveDeviationAlarmType_h__

#include <boost/shared_ptr.hpp>
#include "OpcUaStackCore/Base/os.h"
#include "OpcUaStackCore/BuildInTypes/BuildInTypes.h"
#include "OpcUaStackCore/StandardEventType/ExclusiveLimitAlarmType.h"

namespace OpcUaStackCore
{
    
    class DLLEXPORT ExclusiveDeviationAlarmType
    : public ExclusiveLimitAlarmType
    {
      public:
        typedef boost::shared_ptr<ExclusiveDeviationAlarmType> SPtr;
    
        ExclusiveDeviationAlarmType(void);
        virtual ~ExclusiveDeviationAlarmType(void);
        bool setpointNode(OpcUaVariant::SPtr& variable);
        OpcUaVariant::SPtr setpointNode(void);
        
        bool setExclusiveDeviationAlarmType(OpcUaVariant::SPtr& variable);
        OpcUaVariant::SPtr getExclusiveDeviationAlarmType(void);
        
        
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
