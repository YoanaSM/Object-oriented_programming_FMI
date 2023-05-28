#pragma once
#include <iostream>
#include <memory>

template <typename T>
class SharedPtr
{
	template <typename T> friend class WeakPtr;

	struct Counter
	{
		unsigned useCount = 0;
		unsigned weakCount = 0;

		void removeSharedPtr()
		{
			useCount--;
			if (useCount == 0)
				weakCount--;
		}
		void removeWeakPtr()
		{
			weakCount--;
		}

		void addSharedPtr()
		{
			useCount++;
			if (useCount == 1)
				weakCount++;
		}

		void addWeakPtr()
		{
			weakCount++;
		}

	};

	T* data;
	Counter* counter;

	void free();
	void copyFrom(const SharedPtr<T>& other);

public:
	SharedPtr();
	SharedPtr(T* data);

	SharedPtr(const SharedPtr<T>& other);
	SharedPtr& operator=(const SharedPtr<T>& other);

	const T& operator*() const;
	T& operator*();
	const T* operator->() const;
	T* operator->();

	~SharedPtr();
};

template <typename T>
void SharedPtr<T>::free()
{
	if(data == nullptr && counter == nullptr)
		return;

	counter->removeSharedPtr();

	if (counter->useCount == 0) 
		delete data;

	if (counter->weakCount == 0)
		delete counter;
}

template <typename T>
void SharedPtr<T>::copyFrom(const SharedPtr<T>& other)
{
	data = other.data;
	counter = other.counter;
	if (counter)
		counter->addSharedPtr();
}

template <typename T>
SharedPtr<T>::SharedPtr()
{
	data = nullptr;
	counter = nullptr;
}

template <typename T>
SharedPtr<T>::SharedPtr(T* data)
{
	this->data = data;
	if (this->data)
	{
		counter = new Counter();
		counter->addSharedPtr();
	}
}

template <typename T>
SharedPtr<T>::SharedPtr(const SharedPtr<T>& other)
{
	copyFrom(other); 
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}

template <typename T>
const T& SharedPtr<T>::operator*() const
{
	if(data == nullptr)
	{
		throw std::runtime_error("Pointer not set");
	}
	return *data;
}

template <typename T>
T& SharedPtr<T>::operator*()
{
	if(data == nullptr)
	{
		throw std::runtime_error("Pointer not set");
	}
	return *data;
}

template <typename T>
SharedPtr<T>::~SharedPtr()
{
	free();
}


template<typename T>
T* SharedPtr<T>::operator->() {
	return data;
}

template<typename T>
const T* SharedPtr<T>::operator->() const {
	return data;
}