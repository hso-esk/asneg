/*
   Copyright 2017 Kai Huebl (kai@huebl-sgh.de)

   Lizenziert gemäß Apache Licence Version 2.0 (die „Lizenz“); Nutzung dieser
   Datei nur in Übereinstimmung mit der Lizenz erlaubt.
   Eine Kopie der Lizenz erhalten Sie auf http://www.apache.org/licenses/LICENSE-2.0.

   Sofern nicht gemäß geltendem Recht vorgeschrieben oder schriftlich vereinbart,
   erfolgt die Bereitstellung der im Rahmen der Lizenz verbreiteten Software OHNE
   GEWÄHR ODER VORBEHALTE – ganz gleich, ob ausdrücklich oder stillschweigend.

   Informationen über die jeweiligen Bedingungen für Genehmigungen und Einschränkungen
   im Rahmen der Lizenz finden Sie in der Lizenz.

   Autor: Aleksey Timin (timin-ayu@nefteavtomatika.ru)
 */

#include "OpcUaStackCore/Filter/LiteralFilterNode.h"

namespace OpcUaStackCore
{
    LiteralFilterNode::LiteralFilterNode(const OpcUaVariant& value)
    {

        value_.copyFrom(*const_cast<OpcUaVariant*>(&value));
        status_ = OpcUaStatusCode::Success;
        operandStatuses_ = std::vector<OpcUaStatusCode>();
    }

    LiteralFilterNode::~LiteralFilterNode()
    {

    }

    bool
	LiteralFilterNode::evaluate(OpcUaVariant& value)
    {
    	value_.copyTo(value);
        return true;
    }

    OpcUaStatusCode&
	LiteralFilterNode::status()
    {
    	return status_;
    }

    std::vector<OpcUaStatusCode>&
	LiteralFilterNode::operandStatuses()
    {
    	return operandStatuses_;
    }
}

