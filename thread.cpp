#include "thread.h"

#include "cpu.h"
#include "cpu_impl.h"
#include "thread_impl.h"

thread::thread(thread_startfunc_t thread_func, void* param) {
	this->impl_ptr = new impl(thread_func, param);
}

thread::~thread() {
	delete impl_ptr;
}

void thread::join() {
	this->impl_ptr->impl_join();
}

// static function ~== class function (as opposed to member function)
// member function is specific to a particular instance of the class (eg. join)
// static/class function is not. it is called "on the classs" as whole
void thread::yield() {
	thread::impl::impl_thread_yield();
}
