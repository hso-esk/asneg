/*
    EventTypeClass: ProgressEventType

    Generated Source Code - please do not change this source code

    EventTypeCodeGenerator Version:
        OpcUaStackCore - 3.0.1

    Autor: Kai Huebl (kai@huebl-sgh.de)
*/

#ifndef __OpcUaStackCore_ProgressEventType_h__
#define __OpcUaStackCore_ProgressEventType_h__

#include <boost/shared_ptr.hpp>
#include "OpcUaStackCore/Base/os.h"
#include "OpcUaStackCore/BuildInTypes/BuildInTypes.h"
#include "OpcUaStackCore/StandardEventType/BaseEventType.h"

namespace OpcUaStackCore
{
    
    class DLLEXPORT ProgressEventType
    : public BaseEventType
    {
      public:
        typedef boost::shared_ptr<ProgressEventType> SPtr;
    
        ProgressEventType(void);
        virtual ~ProgressEventType(void);
        bool setProgressEventType(OpcUaVariant::SPtr& variable);
        OpcUaVariant::SPtr getProgressEventType(void);
        
        
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
