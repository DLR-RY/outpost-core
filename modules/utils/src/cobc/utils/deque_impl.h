/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */
// ----------------------------------------------------------------------------

template<typename T>
cobc::Deque<T>::Deque(T* backendBuffer, size_t n) :
	buffer(backendBuffer), maxSize(n), head(0), tail(1), size(0)
{
}

// ----------------------------------------------------------------------------
template<typename T>
bool
cobc::Deque<T>::isEmpty() const
{
	return (this->size == 0);
}

template<typename T>
bool
cobc::Deque<T>::isFull() const
{
	return (this->size == maxSize);
}

template<typename T>
typename cobc::Deque<T>::Size
cobc::Deque<T>::getSize() const
{
	return this->size;
}

template<typename T>
typename cobc::Deque<T>::Size
cobc::Deque<T>::getMaxSize() const
{
	return maxSize;
}

template<typename T>
typename cobc::Deque<T>::Size
cobc::Deque<T>::getAvailableSpace() const
{
	return (maxSize - size);
}

// ----------------------------------------------------------------------------
template<typename T>
void
cobc::Deque<T>::clear()
{
	this->head = 0;
	this->tail = 1;
	this->size = 0;
}

// ----------------------------------------------------------------------------
template<typename T>
T&
cobc::Deque<T>::getFront()
{
	return this->buffer[this->tail];
}


template<typename T>
const T&
cobc::Deque<T>::getFront() const
{
	return this->buffer[this->tail];
}

// ----------------------------------------------------------------------------
template<typename T>
T&
cobc::Deque<T>::getBack()
{
	return this->buffer[this->head];
}

template<typename T>
const T&
cobc::Deque<T>::getBack() const
{
	return this->buffer[this->head];
}

// ----------------------------------------------------------------------------
template<typename T>
bool
cobc::Deque<T>::append(const T& value)
{
	if (this->isFull()) {
		return false;
	}

	if (this->head >= (maxSize - 1)) {
		this->head = 0;
	}
	else {
		this->head++;
	}

	this->buffer[this->head] = value;
	this->size++;
	return true;
}

template<typename T>
void
cobc::Deque<T>::removeBack()
{
	if (this->head == 0) {
		this->head = maxSize - 1;
	}
	else {
		this->head--;
	}
	this->size--;
}

// ----------------------------------------------------------------------------
template<typename T>
bool
cobc::Deque<T>::prepend(const T& value)
{
	if (this->isFull()) {
		return false;
	}

	if (this->tail == 0) {
		this->tail = maxSize - 1;
	}
	else {
		this->tail--;
	}

	this->buffer[this->tail] = value;
	this->size++;
	return true;
}

template<typename T>
void
cobc::Deque<T>::removeFront()
{
	if (this->tail >= (maxSize - 1)) {
		this->tail = 0;
	}
	else {
		this->tail++;
	}

	this->size--;
}
