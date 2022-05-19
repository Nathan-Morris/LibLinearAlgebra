#include "vec.h"

#pragma once

NMATH_NAMESPACE_BEG

template<uint32_t _Rows, uint32_t _Cols = _Rows, typename _NumT = DefNumT>
class mat
{
public:
	//
	using mat_this = mat<_Rows, _Cols, _NumT>;
	
	template<uint32_t _OpCols, typename _OpNumType = _NumT>
	using mat_this_multiplicable = mat<_Cols, _OpCols, _OpNumType>;
	
	template<typename _OpNumType = _NumT>
	using mat_this_type = mat<_Rows, _Cols, _OpNumType>;

	using mat_row = vec<_Cols, _NumT>;
	
	using mat_col = vec<_Rows, _NumT>;
	
	using iterator = ptr_iterator<mat_row>;
	
	using const_iterator = const iterator;
	//

	static constexpr mat_this identity() {
		static_assert(_Rows == _Cols, "Unable To Make Identity Matrix From Non Square Dimensions");
		mat_this id;
		for (size_t i = 0; i != _Rows; i++) {
			id[i][i] = (_NumT)1;
		}
		return id;
	}

private:
	mat_row mValue[_Rows];

public:
	mat(const _NumT& fill = 0) {
		static_assert(_Rows && _Cols, "Unable To Construct Matrix With Dimension Of Zero");
		if (fill != 0) {
			for (auto& v : this->mValue) {
				v = mat_row(fill);
			}
		}
	}

	mat(const std::initializer_list<mat_row>& values) {
		static_assert(_Rows && _Cols, "Unable To Construct Matrix With Dimension Of Zero");
		size_t i = 0;
		auto iter = values.begin();
		while (iter != values.end() && i != _Rows) {
			this->mValue[i++] = *(iter++);
		}
	}

	uint32_t rowCount() const { return _Rows; }
	uint32_t columnCount() const { return _Cols; }

	iterator begin() { return iterator(this->mValue); }
	iterator end() { return iterator(this->mValue + _Rows); }

	const_iterator cbegin() const { return iterator(this->mValue); }
	const_iterator cend() const { return iterator(this->mValue + _Rows); }

	constexpr bool isSqr() const { return _Rows == _Cols; }

	mat_row rowAt(size_t i) const { 
		assert(i < _Rows);
		return this->mValue[i]; 
	}

	mat_col columnAt(size_t i) const {
		mat_col col;
		for (size_t r = 0; r != _Rows; r++) {
			col[r] = this->mValue[r][i];
		}
		return col;
	}

	mat_row& at(size_t i) {
		assert(i < _Rows);
		return this->mValue[i];
	}

	const mat_row& at(size_t i) const {
		assert(i < _Rows);
		return this->mValue[i];
	}

	mat_this& setRow(size_t i, const mat_row& row) {
		this->at(i) = row; 
		return *this;
	}

	mat_this& setColumn(size_t i, const mat_col& col) {
		for (size_t r = 0; r != _Rows; r++) {
			this->at(r).at(i) = col[r];
		}
		return *this;
	}

	//
	// Row Operations
	//

	mat_this& rowSwap(size_t i, size_t j) {
		std::swap(this->at(i), this->at(j));
		return *this;
	}

	// adds `from` to `into`
	mat_this& rowAdd(size_t into, size_t from) {
		this->at(into) += this->at(from);
		return *this;
	}

	mat_this& rowSub(size_t into, size_t from) {
		this->at(into) -= this->at(from);
		return *this;
	}

	mat_this& rowMult(size_t r, const _NumT& c) {
		this->at(r) *= c;
		return *this;
	}

	mat_this& rowDiv(size_t r, const _NumT& c) {
		this->at(r) /= c;
		return *this;
	}

	//
	//
	//

	_NumT* ptr() { return this->mValue[0].ptr(); }
	const _NumT* ptr() const { return this->mValue[0].ptr(); }

	template<typename _OpNumT = _NumT>
	mat_this& add(const mat_this_type<_OpNumT>& mat) {
		for (uint32_t r = 0; r != _Rows; r++) {
			for (uint32_t c = 0; c != _Cols; c++) {
				this->mValue[r][c] += mat[r][c];
			}
		}
		return *this;
	}

	template<typename _OpNumT = _NumT>
	mat_this& sub(const mat_this_type<_OpNumT>& mat) {
		for (uint32_t r = 0; r != _Rows; r++) {
			for (uint32_t c = 0; c != _Cols; c++) {
				this->mValue[r][c] -= mat[r][c];
			}
		}
		return *this;
	}

	template<size_t _OpCols, typename _OpNumT = _NumT>
	mat<_Rows, _OpCols, _NumT> mult(const mat<_Cols, _OpCols, _OpNumT>& m) const {
		mat<_Rows, _OpCols, _NumT> result;
		for (size_t r = 0; r != _Rows; r++) {
			for (size_t c = 0; c != _OpCols; c++) {
				result[r][c] = this->rowAt(r) * m.columnAt(c);
			}
		}
		return result;
	}

	mat_this& mult_scalar(const _NumT& c) {
		for (auto& r : *this) {
			for (auto& v : r) {
				v = c;
			}
		}
		return *this;
	}

	mat<_Cols, _Rows, _NumT> transpose() const {
		mat<_Cols, _Rows, _NumT> m;
		for (size_t r = 0; r != _Rows; r++) {
			for (size_t c = 0; c != _Cols; c++) {
				m[c][r] = this->at(r).at(c);
			}
		}
		return m;
	}

	mat_this inverse() const {
		static_assert(_Rows == _Cols, "Unable To Inverse A Non-Square Matrix");

		mat_this m = *this;
		mat_this id = mat_this::identity();

		static constexpr uint32_t order = _Rows;

		for (uint32_t i = order - 1; i > 0; i--) {
			if (m[i - 1][0] < m[i][0]) {
				m.rowSwap(i, i - 1);
				id.rowSwap(i, i - 1);
			}
		}

		for (uint32_t i = 0; i != order; i++) {
			for (uint32_t j = 0; j != order; j++) {
				if (i != j) {
					_NumT tmp = m[j][i] / m[i][i];

					m.at(j) -= (m.at(i) * tmp);
					id.at(j) -= (id.at(i) * tmp);
				}
			}
		}

		for (uint32_t i = 0; i != order; i++) {
			id[i] /= m[i][i];
		}

		return id;
	}

	//
	//
	//

	mat_row& operator[](size_t i) { return this->at(i); }
	const mat_row& operator[](size_t i) const { return this->at(i); }

	//
	//
	//

	mat_this& operator+=(const mat_this& m) {
		return this->add(m);
	}

	mat_this& operator-=(const mat_this& m) {
		return this->sub(m);
	}

	template<size_t _OpCols, typename _OpNumT = _NumT>
	mat<_Rows, _OpCols, _NumT> operator*=(const mat<_Cols, _OpCols>& m) {
		return this->mult(m);
	}

	mat_this& operator*=(const _NumT& c) {
		return this->mult_scalar(c);
	}

	mat_this& operator/=(const _NumT& c) {
		return this->mult_scalar(_NumT(1) / c);
	}

	//
	//
	//

	template<typename _OpNumTL, typename _OpNumTR>
	friend mat_this operator+(
		const mat_this_type<_OpNumTL>& l,
		const mat_this_type<_OpNumTR>& r
	) {
		mat_this_type<_OpNumTL> res(l);
		res.add(r);
		return res;
	}

	template<typename _OpNumTL, typename _OpNumTR>
	friend mat_this operator-(
		const mat_this_type<_OpNumTL>& l,
		const mat_this_type<_OpNumTR>& r
	) {
		mat_this_type<_OpNumTL> res(l);
		res.sub(r);
		return res;
	}

	template<size_t _OpCols, typename _OpNumTL, typename _OpNumTR>
	friend mat<_Rows, _OpCols, _OpNumTL> operator*(
		const mat<_Rows, _Cols, _OpNumTL>& l,
		const mat<_Cols, _OpCols, _OpNumTR>& r
	) {
		return l.mult(r);
	}

	//
	//
	//

	template<typename _OpNumTL, typename _OpNumTR>
	friend bool operator==(
		const mat_this_type<_OpNumTL>& l,
		const mat_this_type<_OpNumTR>& r
	) {
		for (size_t i = 0; i != _Rows; i++) {
			if (l.at(i) != r.at(i)) {
				return false;
			}
		}
		return true;
	}

	template<typename _OpNumTL, typename _OpNumTR>
	friend bool operator!=(
		const mat_this_type<_OpNumTL>& l,
		const mat_this_type<_OpNumTR>& r
	) { return !(l == r); }

	//
	//
	//

	friend std::ostream& operator<<(std::ostream& out, const mat_this& m) {
		out.put('[');
		for (size_t i = 0; i != _Rows; i++) {
			if (i) {
				out.put(',').put(' ');
			}
			out << m.at(i);
		}
		return out.put(']');
	}
};

NMATH_NAMESPACE_END