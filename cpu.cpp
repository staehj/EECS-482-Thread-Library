#define _XOPEN_SOURCE

#include "cpu.h"
#include "cpu_impl.h"

#include <ucontext.h>

void cpu::init(thread_startfunc_t thread_func, void* param) {
	this->impl_ptr->impl_init(thread_func, param);
}
