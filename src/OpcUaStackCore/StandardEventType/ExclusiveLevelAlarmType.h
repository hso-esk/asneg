/*
    EventTypeClass: ExclusiveLevelAlarmType

    Generated Source Code - please do not change this source code

    EventTypeCodeGenerator Version:
        OpcUaStackCore - 3.0.1

    Autor: Kai Huebl (kai@huebl-sgh.de)
*/

#ifndef __OpcUaStackCore_ExclusiveLevelAlarmType_h__
#define __OpcUaStackCore_ExclusiveLevelAlarmType_h__

#include <boost/shared_ptr.hpp>
#include "OpcUaStackCore/Base/os.h"
#include "OpcUaStackCore/BuildInTypes/BuildInTypes.h"
#include "OpcUaStackCore/StandardEventType/ExclusiveLimitAlarmType.h"

namespace OpcUaStackCore
{
    
    class DLLEXPORT ExclusiveLevelAlarmType
    : public ExclusiveLimitAlarmType
    {
      public:
        typedef boost::shared_ptr<ExclusiveLevelAlarmType> SPtr;
    
        ExclusiveLevelAlarmType(void);
        virtual ~ExclusiveLevelAlarmType(void);
        bool setExclusiveLevelAlarmType(OpcUaVariant::SPtr& variable);
        OpcUaVariant::SPtr getExclusiveLevelAlarmType(void);
        
        
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
