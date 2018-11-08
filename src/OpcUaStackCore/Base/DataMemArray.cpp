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

   Autor: Kai Huebl (kai@huebl-sgh.de)
 */

#include <cstring>
#include <iostream>
#include "OpcUaStackCore/Base/Log.h"
#include "OpcUaStackCore/Base/DataMemArray.h"

namespace OpcUaStackCore
{

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// DataMemArrayHeader
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	DataMemArrayHeader::DataMemArrayHeader(void)
	{
	}

	DataMemArrayHeader::~DataMemArrayHeader(void)
	{
	}


	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// DataMemArraySlot
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	DataMemArraySlot::DataMemArraySlot(void)
	{
	}

	DataMemArraySlot::~DataMemArraySlot(void)
	{
	}

	void
	DataMemArraySlot::set(const char* buf, uint32_t bufLen)
	{
		char* mem = (char*)this + sizeof(DataMemArraySlot);
		memcpy(mem, buf, bufLen);
	}

	bool
	DataMemArraySlot::get(char* buf, uint32_t& bufLen)
	{
		if (dataSize_ > bufLen) {
			return false;
		}
		char* mem = (char*)this + sizeof(DataMemArraySlot);
		memcpy(buf, mem, dataSize_);
		bufLen = dataSize_;
		return true;
	}

	bool
	DataMemArraySlot::get(char** buf, uint32_t& bufLen)
	{
		*buf = (char*)this + sizeof(DataMemArraySlot);
		bufLen = dataSize_;
		return true;
	}

	bool
	DataMemArraySlot::isStartSlot(void)
	{
		return type_ == 'S';
	}

	bool
	DataMemArraySlot::isEndSlot(void)
	{
		return type_ == 'E';
	}

	bool
	DataMemArraySlot::isFreeSlot(void)
	{
		return type_ == 'F';
	}

	bool
	DataMemArraySlot::isUsedSlot(void)
	{
		return type_ == 'U';
	}

	DataMemArraySlot*
	DataMemArraySlot::next(void)
	{
		if (type_ == 'E') return nullptr;
		DataMemArraySlot* slot = (DataMemArraySlot*)((char*)this + sizeof(DataMemArraySlot) + memSize() + sizeof(uint32_t));
		return slot;
	}

	DataMemArraySlot*
	DataMemArraySlot::last(void)
	{
		if (type_ == 'S') return nullptr;
		uint32_t *memSize = (uint32_t*)((char*)this - sizeof(uint32_t));
		DataMemArraySlot* slot = (DataMemArraySlot*)((char*)this - sizeof(uint32_t) - *memSize - sizeof(DataMemArraySlot));
		return slot;
	}

	uint32_t
	DataMemArraySlot::dataSize(void)
	{
		return dataSize_;
	}

	uint32_t
	DataMemArraySlot::paddingSize(void)
	{
		return paddingSize_;
	}

	uint32_t
	DataMemArraySlot::memSize(void)
	{
		return dataSize_ + paddingSize_;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// DataMemArray
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	uint32_t DataMemArray::minMemorySize_ = 100;

	DataMemArray::DataMemArray(void)
	: debug_(true)
	, dataMemArrayHeader_(nullptr)
	, freeSlotMap_()
	, startMemorySize_(10000)
	, maxMemorySize_(1000000)
	, expandMemorySize_(10000)
	{
	}

	DataMemArray::~DataMemArray(void)
	{
		clear();
	}

	void
	DataMemArray::startMemorySize(uint32_t startMemorySize)
	{
		if (startMemorySize < minMemorySize_) {
			startMemorySize_ = minMemorySize_;
		}
		else {
			startMemorySize_ = startMemorySize;
		}
	}

	uint32_t
	DataMemArray::startMemorySize(void)
	{
		return startMemorySize_;
	}

	void
	DataMemArray::maxMemorySize(uint32_t maxMemorySize)
	{
		if (dataMemArrayHeader_ == nullptr) {
			if (maxMemorySize < startMemorySize_) {
				maxMemorySize_ = startMemorySize_;
				expandMemorySize_ = 0;
			}
			else {
				maxMemorySize_ = maxMemorySize;
			}
		}
		else {
			if (maxMemorySize < dataMemArrayHeader_->actArraySize_) {
				dataMemArrayHeader_->maxMemorySize_ = dataMemArrayHeader_->actArraySize_;
			}
			else {
				dataMemArrayHeader_->maxMemorySize_ = maxMemorySize;
			}
		}
	}

	uint32_t
	DataMemArray::maxMemorySize(void)
	{
		if (dataMemArrayHeader_ == nullptr) {
			return maxMemorySize_;
		}
		else {
			return dataMemArrayHeader_->maxMemorySize_;
		}
	}

	void
	DataMemArray::expandMemorySize(uint32_t expandMemorySize)
	{
		if (expandMemorySize < (sizeof(DataMemArraySlot) + sizeof(uint32_t))) {
			expandMemorySize = (sizeof(DataMemArraySlot) + sizeof(uint32_t));
		}

		if (dataMemArrayHeader_ == nullptr) {
			expandMemorySize_ = expandMemorySize;
		}
		else {
			dataMemArrayHeader_->expandMemorySize_ = expandMemorySize;
		}
	}

	uint32_t
	DataMemArray::expandMemorySize(void)
	{
		return expandMemorySize_;
	}

	uint32_t
	DataMemArray::size(void)
	{
		if (dataMemArrayHeader_ == nullptr) {
			return 0;
		}
		return dataMemArrayHeader_->actArraySize_;
	}

	bool
	DataMemArray::resize(uint32_t arraySize)
	{
		if (dataMemArrayHeader_ == nullptr) {

			if (!createNewMemory(arraySize)) return false;

			return true;
		}

		if (arraySize == dataMemArrayHeader_->actArraySize_) {
			return true;
		}

		if (arraySize < dataMemArrayHeader_->actArraySize_) {
			return decreaseArraySize(arraySize);
		}
		else {
			return increaseArraySize(arraySize);
		}

		return true;
	}

	bool
	DataMemArray::unset(uint32_t idx)
	{
		if (dataMemArrayHeader_ == nullptr) {
			return false;
		}
		if (idx >= dataMemArrayHeader_->actArraySize_) {
			return false;
		}

		//
		// get slot
		//
		char* mem = (char*)dataMemArrayHeader_ + dataMemArrayHeader_->actMemorySize_;
		uint32_t* pos = (uint32_t*)(mem - ((idx+1) * sizeof(uint32_t)));
		if (*pos == 0) {
			return true;
		}
		DataMemArraySlot* slot = posToSlot(*pos);
		DataMemArraySlot* lastSlot = slot->last();
		DataMemArraySlot* nextSlot = slot->next();


		//
		// release slot
		//
		uint32_t freeSize = slot->memSize();

		if (nextSlot != nullptr && nextSlot->type_ == 'F') {
			freeSize += (nextSlot->memSize() + sizeof(DataMemArraySlot) + sizeof(uint32_t));

			DataMemArraySlot::Map::iterator it;
			it = freeSlotMap_.find(ptrToPos((char*)nextSlot));
			freeSlotMap_.erase(it);
		}

		if (lastSlot != nullptr && lastSlot->type_ == 'F') {
			freeSize += (lastSlot->memSize() + sizeof(DataMemArraySlot) + sizeof(uint32_t));

			DataMemArraySlot::Map::iterator it;
			it = freeSlotMap_.find(ptrToPos((char*)lastSlot));
			freeSlotMap_.erase(it);

			slot = lastSlot;
		}

		slot = createNewSlot((char*)slot, 'F', freeSize);
		freeSlotMap_.insert(std::make_pair(ptrToPos((char*)slot), slot));

		//
		// release array element
		//
		*pos = 0;

		return true;
	}

	bool
	DataMemArray::set(uint32_t idx, const char* buf, uint32_t bufLen)
	{
		if (buf == nullptr) {
			return false;
		}
		if (bufLen == 0) {
			return false;
		}
		if (dataMemArrayHeader_ == nullptr) {
			return false;
		}
		if (idx >= dataMemArrayHeader_->actArraySize_) {
			return false;
		}

		//
		// find free memory slot
		//
		DataMemArraySlot* slot = nullptr;
		DataMemArraySlot::Map::iterator it;
		for (it = freeSlotMap_.begin(); it != freeSlotMap_.end(); it++) {
			DataMemArraySlot* tmpSlot = it->second;
			if (tmpSlot->dataSize() >= bufLen) {
				slot = tmpSlot;
				break;
			}
		}
		if (slot == nullptr) {
			if (increaseMemory(bufLen)) {
				return set(idx, buf, bufLen);
			}
			return false;
		}

		//
		// check available memory in slot
		//
		freeSlotMap_.erase(it);
		if ((bufLen + sizeof(DataMemArraySlot) + sizeof(uint32_t)) >= slot->dataSize()) {
			// use the entire slot
			uint32_t padding = slot->dataSize() - bufLen;

			slot = createNewSlot((char*)slot, 'U', bufLen, padding);
		}
		else {
			// split slot
			DataMemArraySlot* freeSlot;
			uint32_t freeSlotSize = slot->dataSize() - bufLen - sizeof(DataMemArraySlot) - sizeof(uint32_t);

			slot = createNewSlot((char*)slot, 'U', bufLen, 0);
			freeSlot = createNewSlot((char*)slot + bufLen + sizeof(DataMemArraySlot) + sizeof(uint32_t), 'F', freeSlotSize);
			freeSlotMap_.insert(std::make_pair(ptrToPos((char*)freeSlot), freeSlot));
		}

		//
		// set memory and array index
		//
		slot->set(buf, bufLen);
		setIndex(idx, ptrToPos((char*)slot));

		return true;
	}

	bool
	DataMemArray::set(uint32_t idx, const std::string& value)
	{
		return set(idx, value.c_str(), value.length());
	}

	bool
	DataMemArray::get(uint32_t idx, char* buf, uint32_t& bufLen)
	{
		if (dataMemArrayHeader_ == nullptr) {
			return false;
		}
		if (idx >= dataMemArrayHeader_->actArraySize_) {
			return false;
		}

		char* mem = (char*)dataMemArrayHeader_ + dataMemArrayHeader_->actMemorySize_;
		uint32_t* pos = (uint32_t*)(mem - ((idx+1) * sizeof(uint32_t)));
		if (*pos == 0) {
			return false;
		}
		DataMemArraySlot* slot = posToSlot(*pos);

		return slot->get(buf, bufLen);
	}

	bool
	DataMemArray::get(uint32_t idx, char** buf, uint32_t& bufLen)
	{
		if (dataMemArrayHeader_ == nullptr) {
			return false;
		}
		if (idx >= dataMemArrayHeader_->actArraySize_) {
			return false;
		}

		char* mem = (char*)dataMemArrayHeader_ + dataMemArrayHeader_->actMemorySize_;
		uint32_t* pos = (uint32_t*)(mem - ((idx+1) * sizeof(uint32_t)));
		if (*pos == 0) {
			return false;
		}
		DataMemArraySlot* slot = posToSlot(*pos);

		return slot->get(buf, bufLen);
	}

	bool
	DataMemArray::get(uint32_t idx, std::string& value)
	{
		char *buf;
		uint32_t bufLen;

		if (!get(idx, &buf, bufLen)) {
			return false;
		}

		value = std::string(buf, bufLen);
		return true;
	}

	bool
	DataMemArray::exist(uint32_t idx)
	{
		if (dataMemArrayHeader_ == nullptr) {
			return false;
		}

		char* mem = (char*)dataMemArrayHeader_ + dataMemArrayHeader_->actMemorySize_;
		uint32_t* pos = (uint32_t*)(mem - ((idx+1) * sizeof(uint32_t)));
		if (*pos == 0) {
			return false;
		}

		return true;
	}

	void
	DataMemArray::clear(void)
	{
		if (dataMemArrayHeader_ != nullptr) {
			char* mem = (char*)dataMemArrayHeader_;
			delete [] mem;
			dataMemArrayHeader_ = nullptr;
		}
		freeSlotMap_.clear();

		startMemorySize_ = 10000;
		maxMemorySize_ = 1000000;
		expandMemorySize_ = 10000;
	}

	void
	DataMemArray::log(void)
	{
		logHeader();
		logSlot();
		logArray();
	}

	void
	DataMemArray::logHeader(void)
	{
		if (dataMemArrayHeader_ == nullptr) {
			return;
		}

		Log(Debug, "Header")
		    .parameter("MaxMemorySize", dataMemArrayHeader_->maxMemorySize_)
			.parameter("ExpandMemorySize", dataMemArrayHeader_->expandMemorySize_)
			.parameter("ActMemorySize", dataMemArrayHeader_->actMemorySize_)
			.parameter("ActArraySize", dataMemArrayHeader_->actArraySize_);
	}

	void
	DataMemArray::logSlot(void)
	{
		if (dataMemArrayHeader_ == nullptr) {
			return;
		}

		DataMemArraySlot* slot = firstSlot();
		if (slot == nullptr) {
			return;
		}

		do {
			Log(Debug, "Slot")
				.parameter("Type", slot->type_)
				.parameter("Pos", ptrToPos((char*)slot))
				.parameter("DataSize", slot->dataSize())
				.parameter("PaddingSize", slot->paddingSize())
				.parameter("MemSize", slot->memSize());
			slot = slot->next();
		} while (slot != nullptr);
	}

	void
	DataMemArray::logArray(void)
	{
		if (dataMemArrayHeader_ == nullptr) {
			return;
		}

		char* mem = (char*)dataMemArrayHeader_ + dataMemArrayHeader_->actMemorySize_;
		for (uint32_t idx = 0; idx < dataMemArrayHeader_->actArraySize_; idx++) {
			uint32_t* pos = (uint32_t*)(mem - ((idx+1) * sizeof(uint32_t)));
			if (*pos != 0) {
				DataMemArraySlot* slot = posToSlot(*pos);

				Log(Debug, "Array")
					.parameter("Idx", idx)
					.parameter("Pos", *pos)
					.parameter("Len", slot->dataSize_);
			}
		}
	}

	void
	DataMemArray::logFreeSlots(void)
	{
		if (dataMemArrayHeader_ == nullptr) {
			return;
		}

		DataMemArraySlot::Map::iterator it;
		for (it = freeSlotMap_.begin(); it != freeSlotMap_.end(); it++) {
			DataMemArraySlot* slot = it->second;

			Log(Debug, "FreeSlots")
				.parameter("Pos", it->first)
				.parameter("Len", slot->dataSize());
		}
	}

	bool
	DataMemArray::setMemoryBuf(char* memBuf, uint32_t memLen)
	{
		if (memBuf == nullptr) {
			return false;
		}
		if (dataMemArrayHeader_ != nullptr) {
			clear();
		}

		char* tmpMem = new char [memLen];
		memcpy(tmpMem, memBuf, memLen);

		dataMemArrayHeader_ = (DataMemArrayHeader*)tmpMem;

		//
		// create free slot list
		//
		freeSlotMap_.clear();
		DataMemArraySlot* slot = (DataMemArraySlot*)((char*)dataMemArrayHeader_ + sizeof(DataMemArrayHeader));
		while (slot->type_ != 'E') {
			if (slot->type_ == 'F') {
				freeSlotMap_.insert(std::make_pair(ptrToPos((char*)slot), slot));
			}
			slot = slot->next();
		}

		return true;
	}

	bool
	DataMemArray::getMemoryBuf(char** memBuf, uint32_t* memLen)
	{
		if (dataMemArrayHeader_ == nullptr) {
			return false;
		}

		*memBuf = (char*)dataMemArrayHeader_;
		*memLen = dataMemArrayHeader_->actMemorySize_;

		return true;
	}

	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	//
	// private function
	//
	// ------------------------------------------------------------------------
	// ------------------------------------------------------------------------
	DataMemArraySlot*
	DataMemArray::posToSlot(uint32_t pos)
	{
		if (dataMemArrayHeader_ == nullptr) {
			return nullptr;
		}

		return (DataMemArraySlot*)((char*)dataMemArrayHeader_ + pos);
	}

	uint32_t
	DataMemArray::ptrToPos(char* ptr)
	{
		if (dataMemArrayHeader_ == nullptr) return 0;
		return (ptr - (char*)dataMemArrayHeader_);
	}

	DataMemArraySlot*
	DataMemArray::firstSlot(void)
	{
		if (dataMemArrayHeader_ == nullptr) {
			return nullptr;
		}

		return (DataMemArraySlot*)((char*)dataMemArrayHeader_ + sizeof(DataMemArrayHeader));
	}

	void
	DataMemArray::setIndex(uint32_t idx, uint32_t value)
	{
		if (dataMemArrayHeader_ == nullptr) {
			return;
		}

		char* mem = (char*)dataMemArrayHeader_ + dataMemArrayHeader_->actMemorySize_;
		uint32_t* pos = (uint32_t*)(mem - ((idx+1) * sizeof(uint32_t)));
		*pos = value;
	}

	void
	DataMemArray::getIndex(uint32_t idx, uint32_t& value)
	{
		if (dataMemArrayHeader_ == nullptr) {
			return;
		}

		char* mem = (char*)dataMemArrayHeader_ + dataMemArrayHeader_->actMemorySize_;
		uint32_t* pos = (uint32_t*)(mem - ((idx+1) * sizeof(uint32_t)));
		value = *pos;
	}

	bool
	DataMemArray::createNewMemory(uint32_t arraySize)
	{
		//
		// calculate used buffer size
		//
		// HEAD  B-Slot  F-Slot  E-Slot  Array
		//
		uint32_t usedMemorySize = sizeof(DataMemArrayHeader) +
			3*(sizeof(DataMemArraySlot) + sizeof(uint32_t)) +
			arraySize * sizeof(char*) +
			minMemorySize_;

		//
		// check max memory size
		//
		if (maxMemorySize_ != 0 && usedMemorySize > maxMemorySize_) {
			Log(Error, "allocate memory error - used memory size is bigger then max memory size")
				.parameter("Id", this)
				.parameter("UsedMemorySize", usedMemorySize)
				.parameter("MaxMemorySize", maxMemorySize_);
			return false;
		}

		//
		// calculate actual memory size
		//
		uint32_t actMemorySize = startMemorySize_;
		while (usedMemorySize > actMemorySize) actMemorySize += expandMemorySize_;
		uint32_t dataSize = actMemorySize -
			sizeof(DataMemArrayHeader) -
			(3*(sizeof(DataMemArraySlot) + sizeof(uint32_t))) -
			(arraySize * sizeof(uint32_t));

		//
		// allocate memory and create management structures
		//
		if (debug_) {
			Log(Debug, "create new memory")
				.parameter("Id", this)
				.parameter("ArraySize", arraySize)
				.parameter("MemorySize", actMemorySize)
				.parameter("DataSize", dataSize);
		}

		char* mem = new char[actMemorySize];
		memset(mem, 0x0, actMemorySize);

		// create header
		dataMemArrayHeader_ = (DataMemArrayHeader*)mem;
		dataMemArrayHeader_->eye_[0] = 'H';
		dataMemArrayHeader_->eye_[1] = 'E';
		dataMemArrayHeader_->eye_[2] = 'A';
		dataMemArrayHeader_->eye_[3] = 'D';
		dataMemArrayHeader_->maxMemorySize_ = maxMemorySize_;
		dataMemArrayHeader_->expandMemorySize_ = expandMemorySize_;
		dataMemArrayHeader_->actMemorySize_ = actMemorySize;
		dataMemArrayHeader_->actArraySize_ = arraySize;

		// create start slot
		mem += sizeof(DataMemArrayHeader);
		createNewSlot(mem, 'S', 0);

		// create free slot
		mem += sizeof(DataMemArraySlot) + sizeof(uint32_t);
		createNewSlot(mem, 'F', dataSize);
		freeSlotMap_.insert(std::make_pair(ptrToPos(mem), (DataMemArraySlot*)mem));

		// create end slot
		mem += sizeof(DataMemArraySlot) + sizeof(uint32_t) + dataSize;
		createNewSlot(mem, 'E', 0);

		return true;
	}

	bool
	DataMemArray::increaseMemory(uint32_t size)
	{
		if (dataMemArrayHeader_ == nullptr) {
			return false;
		}

		// calcualte new memory size
		uint32_t usedMemorySize = dataMemArrayHeader_->actMemorySize_ + size + sizeof(DataMemArraySlot) + sizeof(uint32_t);

		//
		// check max memory size
		//
		if (maxMemorySize_ != 0 && usedMemorySize > dataMemArrayHeader_->maxMemorySize_) {
			Log(Error, "increase memory error - used memory size is bigger then max memory size")
				.parameter("Id", this)
				.parameter("UsedMemorySize", usedMemorySize)
				.parameter("MaxMemorySize", dataMemArrayHeader_->maxMemorySize_);
			return false;
		}

		//
		// calculate actual memory size
		//
		uint32_t newMemorySize = dataMemArrayHeader_->actMemorySize_;
		while (usedMemorySize > newMemorySize) newMemorySize += expandMemorySize_;
		uint32_t diffMemorySize = newMemorySize - dataMemArrayHeader_->actMemorySize_;
		uint32_t arrayMemorySize = dataMemArrayHeader_->actArraySize_ * sizeof(uint32_t);
		uint32_t headerSlotMemorySize = dataMemArrayHeader_->actMemorySize_ - arrayMemorySize;

		//
		// create new memory an copy old memory to new memory
		//
		char* mem = new char [newMemorySize];
		memcpy(mem, (char*)dataMemArrayHeader_, headerSlotMemorySize);
		memcpy(&mem[newMemorySize-arrayMemorySize], (char*)dataMemArrayHeader_ + headerSlotMemorySize , arrayMemorySize);

		delete [] (char*)dataMemArrayHeader_;
		dataMemArrayHeader_ = (DataMemArrayHeader*)mem;
		dataMemArrayHeader_->actMemorySize_ = newMemorySize;

		//
		// create new free slot
		//
		char *endSlotMem = (char*)dataMemArrayHeader_ + headerSlotMemorySize - sizeof(DataMemArraySlot) - sizeof(uint32_t);
		DataMemArraySlot* actSlot = (DataMemArraySlot*)endSlotMem;
		DataMemArraySlot* lastSlot = actSlot->last();

		if (lastSlot->type_ == 'F') {
			uint32_t newSlotSize = lastSlot->dataSize() + diffMemorySize;
			actSlot = createNewSlot((char*)lastSlot, 'F', newSlotSize);
			createNewSlot((char*)actSlot->next(), 'E', 0);
		}
		else {
			uint32_t newSlotSize = diffMemorySize - sizeof(DataMemArraySlot) - sizeof(uint32_t);
			actSlot = createNewSlot((char*)actSlot, 'F', newSlotSize);
			createNewSlot((char*)actSlot->next(), 'E', 0);
		}

		//
		// create free slot list
		//
		freeSlotMap_.clear();
		DataMemArraySlot* slot = (DataMemArraySlot*)((char*)dataMemArrayHeader_ + sizeof(DataMemArrayHeader));
		while (slot->type_ != 'E') {
			if (slot->type_ == 'F') {
				freeSlotMap_.insert(std::make_pair(ptrToPos((char*)slot), slot));
			}
			slot = slot->next();
		}

		return true;
	}

	DataMemArraySlot*
	DataMemArray::createNewSlot(char* mem, char type, uint32_t size, uint32_t paddingSize)
	{
		// create header
		DataMemArraySlot* dataMemArraySlot;
		dataMemArraySlot = (DataMemArraySlot*)mem;
		dataMemArraySlot->eye_[0] = 'S';
		dataMemArraySlot->eye_[1] = 'L';
		dataMemArraySlot->eye_[2] = 'O';
		dataMemArraySlot->eye_[3] = 'T';
		dataMemArraySlot->type_ = type;
		dataMemArraySlot->dataSize_ = size;
		dataMemArraySlot->paddingSize_ = paddingSize;

		uint32_t* sizeTag = (uint32_t*)(mem + sizeof(DataMemArraySlot) + dataMemArraySlot->memSize());
		*sizeTag = dataMemArraySlot->memSize();

		return dataMemArraySlot;
	}

	bool
	DataMemArray::increaseArraySize(uint32_t arraySize)
	{
		char* memEnd = (char*)dataMemArrayHeader_ + dataMemArrayHeader_->actMemorySize_;
		uint32_t diffMemorySize = (arraySize - dataMemArrayHeader_->actArraySize_) * sizeof(uint32_t);
		uint32_t arrayMemorySize = dataMemArrayHeader_->actArraySize_ * sizeof(uint32_t);
		uint32_t headerSlotMemorySize = dataMemArrayHeader_->actMemorySize_ - arrayMemorySize;

		// get last two slots
		char *endSlotMem = (char*)dataMemArrayHeader_ + headerSlotMemorySize - sizeof(DataMemArraySlot) - sizeof(uint32_t);
		DataMemArraySlot* actSlot = (DataMemArraySlot*)endSlotMem;
		DataMemArraySlot* lastSlot = actSlot->last();

		if (lastSlot->type_ != 'F') {
			if (increaseMemory(1)) {
				return increaseArraySize(arraySize);
			}
			return false;
		}
		if (lastSlot->dataSize() < diffMemorySize) {
			if (increaseMemory(1)) {
				return increaseArraySize(arraySize);
			}
			return false;
		}

		lastSlot->dataSize_ -= diffMemorySize;
		DataMemArraySlot* slot = lastSlot->next();
		createNewSlot((char*)slot, 'E', 0);

		char* mem = (char*)slot + sizeof(DataMemArraySlot) + sizeof(uint32_t);
		memset(mem, 0x00, diffMemorySize);
		dataMemArrayHeader_->actArraySize_ = arraySize;

		return true;
	}

	bool
	DataMemArray::decreaseArraySize(uint32_t arraySize)
	{
		char* memEnd = (char*)dataMemArrayHeader_ + dataMemArrayHeader_->actMemorySize_;
		uint32_t diffMemorySize = (dataMemArrayHeader_->actArraySize_ - arraySize) * sizeof(uint32_t);
		uint32_t arrayMemorySize = dataMemArrayHeader_->actArraySize_ * sizeof(uint32_t);
		uint32_t headerSlotMemorySize = dataMemArrayHeader_->actMemorySize_ - arrayMemorySize;

		// remove entries
		for (uint32_t idx = arraySize; idx < dataMemArrayHeader_->actMemorySize_; idx++) {
			unset(idx);
		}
		dataMemArrayHeader_->actArraySize_ = arraySize;

		//
		// move end slot
		//
		char *endSlotMem = (char*)dataMemArrayHeader_ + headerSlotMemorySize - sizeof(DataMemArraySlot) - sizeof(uint32_t);
		DataMemArraySlot* actSlot = (DataMemArraySlot*)endSlotMem;
		DataMemArraySlot* lastSlot = actSlot->last();

		if (lastSlot->type_ == 'U') {
			lastSlot->paddingSize_ += diffMemorySize;
		}
		else {
			lastSlot->dataSize_ += diffMemorySize;
		}
		DataMemArraySlot* slot = lastSlot->next();
		createNewSlot((char*)slot, 'E', 0);

		return true;
	}

}
