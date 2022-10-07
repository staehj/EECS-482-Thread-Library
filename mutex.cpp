#define _XOPEN_SOURCE

#include "mutex.h"
#include "mutex_impl.h"

mutex::mutex() {
	this->impl_ptr = new impl;
}

mutex::~mutex() {
	delete impl_ptr;
}

void mutex::lock() {
	this->impl_ptr->impl_lock();
}

void mutex::unlock() {
	this->impl_ptr->impl_unlock();
}