#pragma once
#include "manual_ptr.hpp"

#include <stdexcept>

template <class T, class... Args>
manual_ptr<T> make_manual(Args&&... args) {
	return manual_ptr<T>(new T(std::forward<Args>(args)...));
}

template <class T>
manual_ptr<T>::manual_ptr(std::nullptr_t) :
	m_data(0) {
}

template <class T>
manual_ptr<T>::manual_ptr(T* adopt_ptr) :
	m_data(reinterpret_cast<std::uintptr_t>(adopt_ptr) | std::uintptr_t(1)) {
}

template <class T>
manual_ptr<T>::manual_ptr(const manual_ptr& other) :
	m_data(other.m_data & ~std::uintptr_t(1)) {
}

template <class T>
manual_ptr<T>::manual_ptr(manual_ptr&& other) :
	m_data(std::move(other.m_data)) {
	other.m_data &= ~std::uintptr_t(1);
}

template <class T>
manual_ptr<T>::manual_ptr(manual_ptr& other, bool transfer) :
	m_data(other.m_data & ~std::uintptr_t(!transfer)) {
	other.m_data &= ~std::uintptr_t(transfer);
}

template <class T>
manual_ptr<T>& manual_ptr<T>::operator=(const manual_ptr& other) {
	reset();
	m_data = other.m_data & ~std::uintptr_t(1);
}

template <class T>
manual_ptr<T>& manual_ptr<T>::operator=(manual_ptr&& other) {
	reset();
	m_data = std::move(other.m_data);
	other.m_data &= ~std::uintptr_t(1);
}

template <class T>
manual_ptr<T>::~manual_ptr() {
	reset();
}

template <class T>
bool manual_ptr<T>::transfer_from(manual_ptr& other) {
	if (get != other.get())
		throw std::invalid_argument("manual_ptr: cannot transfer between different pointers");

	// Some hoop jumping to avoid errors when trasfering from myself

	// Store other's deleter (0 or 1)
	std::uintptr_t deleter = other.has_deleter();

	// Remove other's deleter
	other.m_data &= ~std::uintptr_t(1);

	// Assign deleter if 1, keep if deleter = 0
	m_data |= deleter;

	return has_deleter();
}

template <class T>
bool manual_ptr<T>::transfer_to(manual_ptr& other) {
	return other.transfer_from(*this);
}

template <class T>
void manual_ptr<T>::reset(std::nullptr_t) {
	if (has_deleter()) delete get();
	m_data = 0;
}

template <class T>
void manual_ptr<T>::reset(manual_ptr& other, bool transfer) {
	if (has_deleter()) delete get();

	m_data = other.m_data & ~std::uintptr_t(!transfer);
	other.m_data &= ~std::uintptr_t(transfer);
}

template <class T>
void manual_ptr<T>::reset(T* adopt_ptr) {
	if (has_deleter()) delete get();
	m_data = reinterpret_cast<std::uintptr_t>(adopt_ptr) | std::uintptr_t(1);
}

template<class T>
inline T* manual_ptr<T>::release() {
	// If there's no value stored, return nullptr
	if (m_data == 0) return nullptr;

	// If there's a value but manual_ptr is not responsible for deleting,
	// cannot release
	if (!has_deleter()) throw std::runtime_error(
		"manual_ptr::release: cannot release while not holding the deleter");

	// Remove deleter and return value
	m_data &= ~std::uintptr_t(1);
	return get();
}

template<class T>
inline manual_ptr<T>::operator bool() const noexcept {
	return !empty();
}

template<class T>
inline bool manual_ptr<T>::empty() const noexcept {
	return m_data == 0;
}

template <class T>
T* manual_ptr<T>::get() const noexcept {
	return reinterpret_cast<T*>(m_data & ~std::uintptr_t(1));
}

template <class T>
T* manual_ptr<T>::operator->() const noexcept {
	return get();
}

template <class T>
T& manual_ptr<T>::operator*() const {
	return *get();
}

template <class T>
bool manual_ptr<T>::has_deleter() const noexcept {
	return m_data & std::uintptr_t(1);
}