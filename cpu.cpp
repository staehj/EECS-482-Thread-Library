#include "cpu.h"
#include "cpu_impl.h"

#include <ucontext.h>

void cpu::init(thread_startfunc_t body, void* arg) {
    impl_ptr->impl_init(body, arg);
}
