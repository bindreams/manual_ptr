#pragma once
#include <cstdint>

// manual_ptr is similar to shared_ptr, but the deleting responsibility must be
// transferred manually between pointers.
// Use this when you need shared_ptr-like behavior in perfomance-critical cases.
// - When creating a manual_ptr with a pointer to adopt, it automatically assumes
// deleting responsibility.
// - When copying manual_ptr, responsibility is never transferred.
// - When moving manual_ptr, responsibility is always transferred.
// - When using special ctor or reset(), responsibility is transferred according
// to a passed bool parameter.
// Deleter is activated on destruction, assignment or reset.
template <class T>
class manual_ptr {
private:
	std::uintptr_t m_data;

public:
	manual_ptr(std::nullptr_t = nullptr);
	explicit manual_ptr(T* adopt_ptr);
	manual_ptr(const manual_ptr& other);
	manual_ptr(manual_ptr&& other);
	manual_ptr(manual_ptr& other, bool transfer);

	manual_ptr& operator=(const manual_ptr& other);
	manual_ptr& operator=(manual_ptr&& other);

	~manual_ptr();

	bool transfer_from(manual_ptr& other);
	bool transfer_to(manual_ptr& other);

	void reset(std::nullptr_t = nullptr);
	void reset(manual_ptr& other, bool transfer);
	void reset(T* adopt_ptr);

	T* release();

	explicit operator bool() const noexcept;
	bool empty() const noexcept;

	T* get() const noexcept;
	T* operator->() const noexcept;
	T& operator*() const;
	bool has_deleter() const noexcept;
};

template <class T, class... Args>
manual_ptr<T> make_manual(Args&&... args);

#include "manual_ptr.inl"