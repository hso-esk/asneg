#include "OpcUaStackCore/ServiceSet/VariableAttributes.h"

namespace OpcUaStackCore
{

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// VariableAttributes
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	VariableAttributes::VariableAttributes(void)
		: ExtensibleParameterBase()
		, specifiedAttributes_(),
		displayName_(OpcUaLocalizedText::construct()),
		description_(OpcUaLocalizedText::construct()),
		value_(OpcUaDataValue::construct()),
		dataType_(OpcUaNodeId::construct()),
		valueRank_(-1),
		arrayDimensions_(OpcUaUInt32Array::construct()),
		accessLevel_(0),
		userAccessLevel_(0),
		minimumSamplingInterval_(),
		historizing_(false),
		writeMask_(),
		userWriteMask_()
	{
		specifiedAttributes_ |= SpecifiedAttributes_Description;
		specifiedAttributes_ |= SpecifiedAttributes_DisplayName;
		specifiedAttributes_ |= SpecifiedAttributes_Value;
		specifiedAttributes_ |= SpecifiedAttributes_DataType;
		specifiedAttributes_ |= SpecifiedAttributes_ValueRank;
		specifiedAttributes_ |= SpecifiedAttributes_ArrayDimensions;
		specifiedAttributes_ |= SpecifiedAttributes_AccessLevel;
		specifiedAttributes_ |= SpecifiedAttributes_UserAccessLevel;
		specifiedAttributes_ |= SpecifiedAttributes_MinimumSamplingInterval;
		specifiedAttributes_ |= SpecifiedAttributes_Historizing;
		specifiedAttributes_ |= SpecifiedAttributes_WriteMask;
		specifiedAttributes_ |= SpecifiedAttributes_UserWriteMask;
	}

	VariableAttributes::~VariableAttributes(void)
	{
	}

	 OpcUaUInt32
     VariableAttributes::specifiedAttributes(void) const
	 {
		 return specifiedAttributes_;
	 }

	void
	VariableAttributes::displayName(
		const OpcUaLocalizedText::SPtr displayNameSPtr)
	{
		displayName_ = displayNameSPtr;		
	}

    OpcUaLocalizedText::SPtr
	VariableAttributes::displayName(void) const
	{
		return displayName_;
	}

	void
	VariableAttributes::description(
		const OpcUaLocalizedText::SPtr descriptionSPtr)
	{
		description_ = descriptionSPtr;
	}

    OpcUaLocalizedText::SPtr
	VariableAttributes::description(void) const
	{
		return description_;
	}

	void
	VariableAttributes::value(
		const OpcUaDataValue::SPtr valueSPtr)
	{
		value_ = valueSPtr;
	}

    OpcUaDataValue::SPtr
	VariableAttributes::value(void) const
	{
		return value_;
	}
		
	void
	VariableAttributes::dataType(
		const OpcUaNodeId::SPtr dataTypeSPtr)
	{
		dataType_ = dataTypeSPtr;
	}

    OpcUaNodeId::SPtr
	VariableAttributes::dataType(void) const
	{
		return dataType_;
	}

		
	void
	VariableAttributes::valueRank(
		const OpcUaInt32 valueRank)
	{
		valueRank_ = valueRank;
	}

    OpcUaInt32
	VariableAttributes::valueRank(void) const
	{
		return valueRank_;
	}
			
	void
	VariableAttributes::arrayDimensions(
		const OpcUaUInt32Array::SPtr arrayDimensionsSPtr)
	{
		arrayDimensions_ = arrayDimensionsSPtr;
	}

    OpcUaUInt32Array::SPtr
	VariableAttributes::arrayDimensions(void) const
	{
		return arrayDimensions_;
	}

			
	void
	VariableAttributes::accessLevel(
		const OpcUaByte accessLevel)
	{
		accessLevel_ = accessLevel;
	}

    OpcUaByte
	VariableAttributes::accessLevel(void) const
	{
		return accessLevel_;
	}

	
	void
	VariableAttributes::userAccessLevel(
		const OpcUaByte userAccessLevel)
	{
		userAccessLevel_ = userAccessLevel;
	}

    OpcUaByte
	VariableAttributes::userAccessLevel(void) const
	{
		return userAccessLevel_;
	}

	
	void
	VariableAttributes::minimumSamplingInterval(
		const Duration minimumSamplingInterval)
	{
		minimumSamplingInterval_ = minimumSamplingInterval;
	}

    Duration
	VariableAttributes::minimumSamplingInterval(void) const
	{
		return minimumSamplingInterval_;
	}


	void VariableAttributes::historizing(
		const OpcUaBoolean historizing)
	{
		historizing_ = historizing;
	}

    OpcUaBoolean
	VariableAttributes::historizing(void) const
	{
		return historizing_;
	}

	void VariableAttributes::writeMask(
		const WriteableAttribute writeMask)
	{
		writeMask_ |= writeMask;
	}

    OpcUaUInt32
	VariableAttributes::writeMask(void) const
	{
		return writeMask_;
	}

	
	void VariableAttributes::userWriteMask(
		const WriteableAttribute userWriteMask)
	{
		userWriteMask_ |= userWriteMask;
	}

    OpcUaUInt32
	VariableAttributes::userWriteMask(void) const
	{
		return userWriteMask_;
	}

	ExtensibleParameterBase::BSPtr 
	VariableAttributes::factory(void)
	{
		return VariableAttributes::construct();
	}
			
	void 
	VariableAttributes::opcUaBinaryEncode(
		std::ostream& os) const
	{
		OpcUaNumber::opcUaBinaryEncode(os, specifiedAttributes_);
		displayName_->opcUaBinaryEncode(os);
		description_->opcUaBinaryEncode(os);
		value_->opcUaBinaryEncode(os);
		dataType_->opcUaBinaryEncode(os);
		OpcUaNumber::opcUaBinaryEncode(os, valueRank_);
		arrayDimensions_->opcUaBinaryEncode(os);
		OpcUaNumber::opcUaBinaryEncode(os, accessLevel_);
		OpcUaNumber::opcUaBinaryEncode(os, userAccessLevel_);
		OpcUaNumber::opcUaBinaryEncode(os, minimumSamplingInterval_);
		OpcUaNumber::opcUaBinaryEncode(os, historizing_);
		OpcUaNumber::opcUaBinaryEncode(os, writeMask_);
		OpcUaNumber::opcUaBinaryEncode(os, userWriteMask_);
	}
	
	void 
	VariableAttributes::opcUaBinaryDecode(
		std::istream& is)
	{
		OpcUaNumber::opcUaBinaryDecode(is, specifiedAttributes_);
		displayName_->opcUaBinaryDecode(is);
		description_->opcUaBinaryDecode(is);
		value_->opcUaBinaryDecode(is);
		dataType_->opcUaBinaryDecode(is);
		OpcUaNumber::opcUaBinaryDecode(is, valueRank_);
		arrayDimensions_->opcUaBinaryDecode(is);
		OpcUaNumber::opcUaBinaryDecode(is, accessLevel_);
		OpcUaNumber::opcUaBinaryDecode(is, userAccessLevel_);
		OpcUaNumber::opcUaBinaryDecode(is, minimumSamplingInterval_);
		OpcUaNumber::opcUaBinaryDecode(is, historizing_);
		OpcUaNumber::opcUaBinaryDecode(is, writeMask_);
		OpcUaNumber::opcUaBinaryDecode(is, userWriteMask_);
	}

}