#include "nmath_global.h"

#include "dim.h"

#pragma once

NMATH_NAMESPACE_BEG

template<const uint32_t _Dim, typename _NumT = DefNumT>
class vec
{
private:
	using iterator = ptr_iterator<_NumT>;
	using const_iterator = const iterator;

private:
	_NumT mValue[_Dim] = { 0 };

public:
	vec(const _NumT& fill = 0) {
		if (fill != 0) {
			for (_NumT& num : this->mValue) { 
				num = fill; 
			}
		}
	}
	
	vec(const std::initializer_list<_NumT>& values) {
		size_t i = 0;
		auto iter = values.begin();
		while (iter != values.end() && i != _Dim) {
			this->mValue[i++] = *(iter++);
		}
	}

	template<typename _OpNumT = _NumT>
	vec(const vec<_Dim, _OpNumT>& v) {
		for (size_t i = 0; i != _Dim; i++) {
			this->mValue[i] = v[i];
		}
	}

	size_t dimensions() const { return _Dim; }

	iterator begin() { return iterator(this->mValue); }
	iterator end() { return iterator(this->mValue + _Dim); }

	const_iterator cbegin() { return iterator(this->mValue); }
	const_iterator cend() { return iterator(&this->mValue[_Dim - 1]); }

	const _NumT& at(size_t i) const
	{
		assert(i < _Dim, "Vector Dimension Out Of Range");
		return this->mValue[i];
	}

	_NumT& at(size_t i) 
	{
		assert(i < _Dim, "Vector Dimension Out Of Range");
		return this->mValue[i];
	}
	
	_NumT& at(dim i)  { return this->at((size_t)i); }
	const _NumT& at(dim i) const { return this->at((size_t)i); }

	_NumT* ptr() { return this->mValue; }
	const _NumT* ptr() const { return this->mValue; }

	template<typename _OpNumT = _NumT>
	vec<_Dim, _NumT>& add(const vec<_Dim, _OpNumT>& v) {
		for (uint32_t i = 0; i != _Dim; i++) {
			this->mValue[i] += v[i];
		}
		return *this;
	}

	template<typename _OpNumT = _NumT>
	vec<_Dim, _NumT>& sub(const vec<_Dim, _OpNumT>& v) {
		for (uint32_t i = 0; i != _Dim; i++) {
			this->mValue[i] -= v[i];
		}
		return *this;
	}

	template<typename _OpNumT = _NumT>
	vec<_Dim, _NumT>& mult(const _OpNumT& c) {
		for (uint32_t i = 0; i != _Dim; i++) {
			this->mValue[i] *= c;
		}
		return *this;
	}

	template<typename _OpNumT = _NumT>
	vec<_Dim, _NumT>& div(const _OpNumT& c) {
		for (uint32_t i = 0; i != _Dim; i++) {
			this->mValue[i] /= c;
		}
		return *this;
	}

	template<typename _OpNumT = _NumT>
	_NumT dot(const vec<_Dim, _OpNumT>& v) const {
		_NumT result = 0;
		for (uint32_t i = 0; i != _Dim; i++) {
			result += this->mValue[i] * v[i];
		}
		return result;
	}

	_NumT magnitude() const {
		_NumT mag = 0;
		for (const _NumT& val : this->mValue) {
			mag += val * val;
		}
		return sqrt(mag);
	}

	vec<_Dim, _NumT>& normalize() {
		_NumT mag = this->magnitude();
		for (_NumT& val : this->mValue) {
			val /= mag;
		}
		return *this;
	}

	//
	// 
	//

	_NumT& operator[](size_t i) { return this->at(i); }
	const _NumT& operator[](size_t i) const { return this->at(i); }

	//
	//
	//

	template<typename _OpNumT>
	vec<_Dim, _NumT>& operator=(const vec<_Dim, _OpNumT>& v) {
		for (size_t i = 0; i != _Dim; i++) {
			this->mValue[i] = v[i];
		}
		return *this;
	}

	vec<_Dim, _NumT>& operator=(const std::initializer_list<_NumT>& values) {
		size_t i = 0;
		auto iter = values.begin();
		while (iter != values.end() && i != _Dim) {
			this->mValue[i++] = *(iter++);
		}
		return *this;
	}

	//
	//
	//

	template<typename _OpNumT = _NumT>
	vec<_Dim, _NumT>& operator+=(const vec<_Dim, _OpNumT>& v) {
		return this->add(v);
	}

	template<typename _OpNumT = _NumT>
	vec<_Dim, _NumT>& operator-=(const vec<_Dim, _OpNumT>& v) {
		return this->sub(v);
	}

	template<typename _OpNumT = _NumT>
	vec<_Dim, _NumT>& operator*=(const _OpNumT& c) {
		return this->mult(c);
	}

	template<typename _OpNumT = _NumT>
	_NumT operator*=(const vec<_Dim, _OpNumT>& v) {
		return this->dot(v);
	}

	template<typename _OpNumT = _NumT>
	vec<_Dim, _NumT>& operator/=(const _OpNumT& c) {
		return this->div(c);
	}

	//
	//
	//

	template<typename _OpNumTL, typename _OpNumTR>
	friend vec<_Dim, _OpNumTL> operator+(
		const vec<_Dim, _OpNumTL>& l,
		const vec<_Dim, _OpNumTR>& r
	) {
		vec<_Dim, _OpNumTL> v(l);
		v.add(r);
		return v;
	}

	template<typename _OpNumTL, typename _OpNumTR>
	friend vec<_Dim, _OpNumTL> operator-(
		const vec<_Dim, _OpNumTL>& l,
		const vec<_Dim, _OpNumTR>& r
		) {
		vec<_Dim, _OpNumTL> v(l);
		v.sub(r);
		return v;
	}

	template<typename _OpNumTL, typename _OpNumTR>
	friend _OpNumTL operator*(
		const vec<_Dim, _OpNumTL>& l,
		const vec<_Dim, _OpNumTR>& r
	) { return l.dot(r); }

	template<typename _OpNumTL, typename _OpNumTR>
	friend vec<_Dim, _OpNumTL> operator*(
		const vec<_Dim, _OpNumTL>& l,
		const _OpNumTR& r
	) {
		vec<_Dim, _OpNumTL> v(l);
		return v.mult(r);
	}

	template<typename _OpNumTL, typename _OpNumTR>
	friend _OpNumTL operator/(
		const vec<_Dim, _OpNumTL>& l,
		const _OpNumTR& r
	) {
		vec<_Dim, _OpNumTL> v(l);
		return v.div(r);
	}

	//
	//
	//

	template<typename _OpNumTL, typename _OpNumTR>
	friend bool operator==(
		const vec<_Dim, _OpNumTL>& l,
		const vec<_Dim, _OpNumTR>& r
	) {
		for (size_t i = 0; i != _Dim; i++) {
			if (l.mValue[i] != r.mValue[i]) {
				return false;
			}
		}
		return true;
	}

	template<typename _OpNumTL, typename _OpNumTR>
	friend bool operator!=(
		const vec<_Dim, _OpNumTL>& l,
		const vec<_Dim, _OpNumTR>& r
	) { return !(l == r); }

	//
	//
	//

	template<typename _OpNumT>
	friend std::ostream& operator<<(
		std::ostream& out, 
		const vec<_Dim, _OpNumT>& v
	) {
		out.put('<');
		for (size_t i = 0; i != _Dim; i++) {
			if (i) {
				out.put(',');
				out.put(' ');
			}
			out << v.mValue[i];
		}
		out.put('>');
		return out;
	}
};

NMATH_NAMESPACE_END