#include "cv.h"
#include "cv_impl.h"

cv::cv() {
	this->impl_ptr = new impl;
}

cv::~cv() {
	delete this->impl_ptr;
}

void cv::wait(mutex& mutex) {
	this->impl_ptr->impl_wait(mutex);
}

void cv::signal() {
	this->impl_ptr->impl_signal();
}

void cv::broadcast() {
	this->impl_ptr->impl_broadcast();
}