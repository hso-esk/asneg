/*
    EventTypeClass: NonExclusiveLevelAlarmType

    Generated Source Code - please do not change this source code

    EventTypeCodeGenerator Version:
        OpcUaStackCore - 3.0.1

    Autor: Kai Huebl (kai@huebl-sgh.de)
*/

#ifndef __OpcUaStackCore_NonExclusiveLevelAlarmType_h__
#define __OpcUaStackCore_NonExclusiveLevelAlarmType_h__

#include <boost/shared_ptr.hpp>
#include "OpcUaStackCore/Base/os.h"
#include "OpcUaStackCore/BuildInTypes/BuildInTypes.h"
#include "OpcUaStackCore/StandardEventType/NonExclusiveLimitAlarmType.h"

namespace OpcUaStackCore
{
    
    class DLLEXPORT NonExclusiveLevelAlarmType
    : public NonExclusiveLimitAlarmType
    {
      public:
        typedef boost::shared_ptr<NonExclusiveLevelAlarmType> SPtr;
    
        NonExclusiveLevelAlarmType(void);
        virtual ~NonExclusiveLevelAlarmType(void);
        bool setNonExclusiveLevelAlarmType(OpcUaVariant::SPtr& variable);
        OpcUaVariant::SPtr getNonExclusiveLevelAlarmType(void);
        
        
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
