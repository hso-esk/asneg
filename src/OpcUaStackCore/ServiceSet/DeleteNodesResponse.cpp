/*
   Copyright 2015 Kai Huebl (kai@huebl-sgh.de)

   Lizenziert gemäß Apache Licence Version 2.0 (die „Lizenz“); Nutzung dieser
   Datei nur in Übereinstimmung mit der Lizenz erlaubt.
   Eine Kopie der Lizenz erhalten Sie auf http://www.apache.org/licenses/LICENSE-2.0.

   Sofern nicht gemäß geltendem Recht vorgeschrieben oder schriftlich vereinbart,
   erfolgt die Bereitstellung der im Rahmen der Lizenz verbreiteten Software OHNE
   GEWÄHR ODER VORBEHALTE – ganz gleich, ob ausdrücklich oder stillschweigend.

   Informationen über die jeweiligen Bedingungen für Genehmigungen und Einschränkungen
   im Rahmen der Lizenz finden Sie in der Lizenz.

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#include "OpcUaStackCore/ServiceSet/DeleteNodesResponse.h"

namespace OpcUaStackCore
{

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// DeleteNodesResponse
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	DeleteNodesResponse::DeleteNodesResponse(void)
	: Object()
	, deleteNodesResultArraySPtr_(constructSPtr<DeleteNodesResultArray>())
	, diagnosticInfoArraySPtr_(constructSPtr<OpcUaDiagnosticInfoArray>())
	{
	}

	DeleteNodesResponse::~DeleteNodesResponse(void)
	{
	}

	void
	DeleteNodesResponse::results(
		const DeleteNodesResultArray::SPtr deleteNodesResultArraySPtr)
	{
		deleteNodesResultArraySPtr_ = deleteNodesResultArraySPtr;
	}

	DeleteNodesResultArray::SPtr
	DeleteNodesResponse::results() const
	{
		return deleteNodesResultArraySPtr_;
	}


	void
	DeleteNodesResponse::diagnosticInfos(const OpcUaDiagnosticInfoArray::SPtr diagnosticInfosSPtr)
	{
		diagnosticInfoArraySPtr_ = diagnosticInfosSPtr;
	}

	OpcUaDiagnosticInfoArray::SPtr
	DeleteNodesResponse::diagnosticInfos(void) const
	{
		return diagnosticInfoArraySPtr_;
	}
		

	void 
	DeleteNodesResponse::opcUaBinaryEncode(std::ostream& os) const
	{
		deleteNodesResultArraySPtr_->opcUaBinaryEncode(os);
		diagnosticInfoArraySPtr_->opcUaBinaryEncode(os);
	}
	
	void 
	DeleteNodesResponse::opcUaBinaryDecode(std::istream& is)
	{
		deleteNodesResultArraySPtr_->opcUaBinaryDecode(is);
		diagnosticInfoArraySPtr_->opcUaBinaryDecode(is);
	}

}
