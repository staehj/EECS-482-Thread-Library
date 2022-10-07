#define _XOPEN_SOURCE

#include "cpu.h"
#include "cpu_impl.h"

cpu::impl::impl() {

}

cpu::impl::~impl() {

}

// Wrapper for thread function to track completion of stream of execution
void cpu::impl::thread_wrapper(thread_startfunc_t body, void* arg) {
    // call thread_function
    // call thread_exit
}

// Called after completion of stream of execution
void cpu::impl::thread_exit() {
    // disable interrupts
    // clear cpu finished_queue (no need to delete explicitly due to smart pointers)
    // add current context to finished_queue
    // if waiting_thread exists for this thread:
        // remove waiting_thread from cpu waiting_set
        // add waiting_thread to cpu ready_queue
    // enable interrupts
}

void* cpu::impl::impl_timer_interrupt_handler() {
	// disable interrupts
    // if !ready_queue.empty():
        // add running_context to ready_queue
        // retrive/pop context from ready_queue
        // assign popped context as running_context
        // set/start popped context
    // enable interrupts
}

void cpu::impl::impl_thread_yield() {
	// is this identical to impl_timer_interrupt_handler?
    // consider edge cases and abstracting this function
}

void cpu::impl::impl_init(thread_startfunc_t body, void* arg) {
    // initialize interrupt vector
    // interrupt_vector_table[IPI] not set intentionally
    interrupt_handler_t interrupt_handler_ptr;
    interrupt_handler_ptr = (interrupt_handler_t) impl_timer_interrupt_handler;
    cpu::self()->interrupt_vector_table[TIMER] = interrupt_handler_ptr;

    // wrap thread_func with thread_wrapper


    // make context with wrapper thread_func


    // assign context as running_context


    // (?) enable interrupt (?) (check invariant for set_context)
    cpu::interrupt_enable();
    // set/start context
}
