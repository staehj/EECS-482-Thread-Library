#pragma once
#include "mutex.h"

class mutex::impl {
public:
	impl();

	~impl();

	void impl_lock();

	void impl_unlock();

private:

};