#pragma once
#include "cv.h"

class cv::impl {
public:
	impl();

	~impl();

	void impl_wait(mutex&);

	void impl_signal();

	void impl_broadcast();

private:
	// Add any private data members to implement this function

};