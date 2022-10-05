#include "cpu.h"
#include "cpu_impl.h"

void cpu::init(thread_startfunc_t thread_func, void* param) {
	this->impl_ptr->impl_init(thread_func, param);
}