#define _XOPEN_SOURCE

#include "cpu.h"
#include "cpu_impl.h"

#include <ucontext.h>

void impl_timer_interrupt_handler() {
	// disable interrupts
    // if !ready_queue.empty():
        // add running_context to ready_queue
        // retrive/pop context from ready_queue
        // assign popped context as running_context
        // set/start popped context
    // enable interrupts
}

void cpu::init(thread_startfunc_t body, void* arg) {
    // initialize interrupt vector
    interrupt_vector_table[TIMER] =  impl_timer_interrupt_handler;

    impl_ptr->impl_init(body, arg);
}
