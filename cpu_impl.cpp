#define _XOPEN_SOURCE

#include "cpu.h"
#include "cpu_impl.h"

#include "context_wrapper.h"
#include "thread_impl.h"

cpu::impl::impl() {

}

cpu::impl::~impl() {

}

// Wrapper for thread function to track completion of stream of execution
void cpu::impl::thread_wrapper(thread_startfunc_t body, void* arg) {
    // call thread_function
    (*body)(arg);

    // call thread_exit
    thread_exit();
}

// Called after completion of stream of execution
void cpu::impl::thread_exit() {
    // disable interrupts
    cpu::interrupt_disable();

    // clear cpu finished_queue (no need to delete explicitly due to smart pointers)
    for (intptr_t = 0)
    // add current context to finished_queue
    // if waiting_thread exists for this thread:
        // remove waiting_thread from cpu waiting_set
        // add waiting_thread to cpu ready_queue

    // enable interrupts
    cpu::interrupt_enable();
}

void cpu::impl::impl_timer_interrupt_handler() {
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

// Assumes interrupt vector is initialized
void cpu::impl::impl_init(thread_startfunc_t body, void* arg) {
    // TODO: wrap thread_func with thread_wrapper
    void (*wrapped_ptr)(thread_startfunc_t, void*);
    wrapped_ptr = thread_wrapper;

    // TODO: make context with wrapper thread_func
    context_wrapper context = thread::impl::set_up_context();

    makecontext(context.context_ptr.get(), (void (*)()) wrapped_ptr, 2, body, arg);

    // TODO: assign context as running_context
    running_context = std::move(context);

    // (?) enable interrupt (?) (check invariant for set_context)
    cpu::interrupt_enable();

    // set/start context
    setcontext(ucontext_ptr.get());
}
