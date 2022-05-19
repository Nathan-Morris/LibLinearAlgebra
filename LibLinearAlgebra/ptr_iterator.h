#pragma once

template<typename T>
class ptr_iterator
{
private:
	T* mPtr;

public:
	ptr_iterator(T* ptr) : mPtr(ptr) { }

	T& operator*() { return *this->mPtr; }
	const T& operator*() const { return *this->mPtr; }

	T* operator->() { return this->mPtr; }
	const T* operator->() const { return this->mPtr; }

	ptr_iterator<T>& operator++() { this->mPtr += 1; return *this; }
	ptr_iterator<T> operator++(int) { ptr_iterator<T> tmp = *this; ++(*this); return tmp; }

	friend bool operator==(const ptr_iterator<T>& l, const ptr_iterator<T>& r) { return l.operator->() == r.operator->(); }
	friend bool operator!=(const ptr_iterator<T>& l, const ptr_iterator<T>& r) { return !(l == r); }
};

