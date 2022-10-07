#define _XOPEN_SOURCE

#include "thread.h"

#include "cpu.h"
#include "cpu_impl.h"
#include "thread_impl.h"

thread::thread(thread_startfunc_t thread_func, void* param) {
	this->impl_ptr = new impl;
}

thread::~thread() {
	delete impl_ptr;
}

void thread::join() {
	this->impl_ptr->impl_join();
}

void thread::yield() {
	cpu::self()->impl_ptr->impl_thread_yield();
}
