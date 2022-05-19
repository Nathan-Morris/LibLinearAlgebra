#include "equnode.h"

#include <functional>

#pragma once

NMATH_NAMESPACE_BEG

template<typename _NumT = DefNumT>
class EquOpInfo {
private:
	static std::map<uint32_t, EquOpInfo> OP_INFO_MAP = {};

private:
	union {
		std::function<_NumT(_NumT)> unary;
		std::function<_NumT(_NumT, _NumT)> binary;
		std::function<_NumT(uint8_t, ...)> function;
	} mCallbacks;

public:
	EquOpInfo(const char* token, std::function<_NumT(_NumT)> un) {
		this->mCallbacks.unary = un;
	}

	EquOpInfo(const char* token, std::function<_NumT(_NumT, _NumT)> bin) {
		this->mCallbacks.binary = bin;
	}

	EquOpInfo(const char* token, std::function<_NumT(uint8_t, ...)> fun) {
		this->mCallbacks.function = fun;
	}


};

NMATH_NAMESPACE_END
