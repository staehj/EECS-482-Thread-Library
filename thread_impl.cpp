#define _XOPEN_SOURCE

#include "thread_impl.h"

#include <memory>
#include <ucontext.h>

#include "shared.h"

thread::impl::impl() {
    // add thread context to cpu ready_queue
}

thread::impl::~impl() {

}

std::unique_ptr<context_wrapper> set_up_context() {
    auto wrapper = std::make_unique<context_wrapper>();
    ucontext_t *new_ucontext_ptr = new ucontext_t;
    char *stack = new char [STACK_SIZE];
    new_ucontext_ptr->uc_stack.ss_sp = stack;
    new_ucontext_ptr->uc_stack.ss_size = STACK_SIZE;
    new_ucontext_ptr->uc_stack.ss_flags = 0;
    new_ucontext_ptr->uc_link = nullptr;

    wrapper->context_ptr = new_ucontext_ptr;
    wrapper->stack_base = stack;
    return wrapper;
}

// Note, if A calls B.join(), then BOTH A and B thread objects must exist
// So we do not have to worry about A or B thread objects being destroyed
void thread::impl::impl_join() {
    // set current(/calling) context as waiting_thread of called thread
        // eg. if A calls B.join --> A is calling thread, B is called thread
    // put calling context into cpu waiting_set
}
// Wrapper for thread function to track completion of stream of execution
void thread::impl::thread_wrapper(thread_startfunc_t body, void* arg) {
    // call thread_function
    (*body)(arg);

    // call thread_exit
    thread_exit();
}

// Called after completion of stream of execution
void thread::impl::thread_exit() {
    // disable interrupts
    cpu::interrupt_disable();

    // clear cpu finished_queue (must delete stack + context_wrapper)
    while (!finished_queue.empty()) {
        // retrive context from finished queue
        auto context = std::move(finished_queue.front());
        finished_queue.pop();

        // delete stack associated with context
        delete[] context->stack_base;

        // delete context_wrapper object
        context.reset();  // TODO: is this equivalent to delete on a raw pointer?
                          // or is the point of smart pointers that this happens automatically?
    }

    // ensure waiting threads (due to join) are moved to ready queue
    if (running_context->waiting_context != nullptr) {
        // remove waiting_thread from cpu waiting_set
        waiting_set.erase(running_context->waiting_context);
        // TODO: how to track waiting thread with unique pointers?
        // In the current implementation, it seems there would be a u_ptr to the waiting thread
        // in BOTH the waiting_set and running_context (context_wrapper).

        // add waiting_thread to cpu ready_queue

    }

    // add current/running context to finished_queue
    finished_queue.push(std::move(running_context));

    // enable interrupts
    cpu::interrupt_enable();
}

void thread::impl::impl_thread_yield() {
	// is this identical to impl_timer_interrupt_handler?
    // consider edge cases and abstracting this function
}