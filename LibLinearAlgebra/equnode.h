#include "nmath_global.h"

#pragma once

NMATH_NAMESPACE_BEG

static constexpr uint32_t operationTokenHash(const char* cstr) {
	uint32_t hash = 5381;
	while (*cstr) {
		hash = ((hash << 5) + hash) + *(cstr++);
	}
	return hash;
}

template<typename _NumT = DefNumT>
class Equnode
{
public:
	enum type : uint8_t {
		OPERATION,
		OPERAND,
		SCOPE
	};

	enum operation_type : uint8_t {
		UNARY,
		BINARY
	};

	enum operand_type : uint8_t {
		VARIABLE,
		VALUE
	};

	enum scope_type : uint8_t {
		INCREMENT,
		DECREMENT
	};

protected:
	uint8_t mType		: 4;
	uint8_t mSubType	: 4;

	union _equnodedata {
		struct _operationinfo {
			uint32_t hash;
		} operation;

		union _operandinfo {
			char variable;
			_NumT value;
		} operand;
	} mData;

public:
	uint8_t type() const { return this->mType; }
	uint8_t subType() const { return this->mSubType; }
};


template<typename _NumT = DefNumT>
class EqunodeOperation : public Equnode<_NumT> {
public:
	EqunodeOperation(const char* token, uint8_t type) {
		this->mType = Equnode<_NumT>::OPERATION;
		this->mSubType = type;
		this->mData.operation.hash = operationTokenHash(token);
	}

	uint32_t hash() const { return this->mData.operation.hash; }
};


template<typename _NumT = DefNumT>
class EqunodeOperand : public Equnode<_NumT> {
public:
	EqunodeOperand(char var) {
		this->mType = Equnode<_NumT>::OPERAND;
		this->mSubType = Equnode<_NumT>::VARIABLE;
		this->mData.operand.variable = var;
	}

	EqunodeOperand(const _NumT& val) {
		this->mType = Equnode<_NumT>::OPERAND;
		this->mSubType = Equnode<_NumT>::VALUE;
		this->mData.operand.value = val;
	}

	_NumT value() const { return this->mData.operand.value; }
	char variable() const { return this->mData.operand.variable; }
};

NMATH_NAMESPACE_END