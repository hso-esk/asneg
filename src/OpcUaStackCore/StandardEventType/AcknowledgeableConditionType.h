/*
    EventTypeClass: AcknowledgeableConditionType

    Generated Source Code - please do not change this source code

    EventTypeCodeGenerator Version:
        OpcUaStackCore - 3.0.1

    Autor: Kai Huebl (kai@huebl-sgh.de)
*/

#ifndef __OpcUaStackCore_AcknowledgeableConditionType_h__
#define __OpcUaStackCore_AcknowledgeableConditionType_h__

#include <boost/shared_ptr.hpp>
#include "OpcUaStackCore/Base/os.h"
#include "OpcUaStackCore/BuildInTypes/BuildInTypes.h"
#include "OpcUaStackCore/StandardEventType/ConditionType.h"

namespace OpcUaStackCore
{
    
    class DLLEXPORT AcknowledgeableConditionType
    : public ConditionType
    {
      public:
        typedef boost::shared_ptr<AcknowledgeableConditionType> SPtr;
    
        AcknowledgeableConditionType(void);
        virtual ~AcknowledgeableConditionType(void);
        bool enabledState(OpcUaVariant::SPtr& variable);
        OpcUaVariant::SPtr enabledState(void);
        
        bool enabledState_Id(OpcUaVariant::SPtr& variable);
        OpcUaVariant::SPtr enabledState_Id(void);
        
        bool ackedState(OpcUaVariant::SPtr& variable);
        OpcUaVariant::SPtr ackedState(void);
        
        bool ackedState_Id(OpcUaVariant::SPtr& variable);
        OpcUaVariant::SPtr ackedState_Id(void);
        
        bool ackedState_TransitionTime(OpcUaVariant::SPtr& variable);
        OpcUaVariant::SPtr ackedState_TransitionTime(void);
        
        bool confirmedState(OpcUaVariant::SPtr& variable);
        OpcUaVariant::SPtr confirmedState(void);
        
        bool confirmedState_Id(OpcUaVariant::SPtr& variable);
        OpcUaVariant::SPtr confirmedState_Id(void);
        
        bool confirmedState_TransitionTime(OpcUaVariant::SPtr& variable);
        OpcUaVariant::SPtr confirmedState_TransitionTime(void);
        
        bool setAcknowledgeableConditionType(OpcUaVariant::SPtr& variable);
        OpcUaVariant::SPtr getAcknowledgeableConditionType(void);
        
        
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
