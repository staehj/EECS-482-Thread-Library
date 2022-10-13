#define _XOPEN_SOURCE

#include "thread_impl.h"

#include <memory>
#include <ucontext.h>

#include "cpu.h"
#include "cpu_impl.h"
#include "shared.h"

thread::impl::impl(thread_startfunc_t thread_func, void* param) {
    // disable interrupts
    cpu::interrupt_disable();
    
    // wrap thread_func with thread_wrapper
    void (*wrapped_func_ptr)(thread_startfunc_t, void*);
    wrapped_func_ptr = thread::impl::thread_wrapper;

    // make context with wrapper thread_func
    auto context = std::move(thread::impl::set_up_context());
    makecontext(context->context_ptr, (void (*)()) wrapped_func_ptr, 2, thread_func, param);

    // add context to ready queue
    ready_queue.push(context);

    // enable interrupts
    cpu::interrupt_enable();
}

thread::impl::~impl() {
    // i dont think this is needed
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
// eg. A calls B.join()
// cpu::self()->running_context === A
// B's context - how to reference?
void thread::impl::impl_join() {
    // disable interrupts
    cpu::interrupt_disable();

    // add current(/calling) context to waiting_queue of called thread
    ucontext_t* temp = cpu::self()->impl_ptr->running_context->context_ptr;
    waiting_queue.push(std::move(cpu::self()->impl_ptr->running_context));

    // retrive/pop context from ready_queue + assign as running_context
    cpu::self()->impl_ptr->running_context = std::move(ready_queue.front());
    ready_queue.pop();

    // save current context and start new context
    swapcontext(temp, cpu::self()->impl_ptr->running_context->context_ptr);
    
    // enable interrupts
    cpu::interrupt_enable();
}

// Wrapper for thread function to track completion of stream of execution
void thread::impl::thread_wrapper(thread_startfunc_t body, void* arg) {
    // TODO: gain clarity on interrupts here
    // enable interrupts
    cpu::interrupt_enable();
    
    // call thread_function
    (*body)(arg);

    // disable interrupts
    cpu::interrupt_disable();

    // call thread_exit
    thread_exit();
}

// Called after completion of stream of execution
// ASSUMES interrupts are disabled
void thread::impl::thread_exit() {
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
    if (/* TODO */) {
        // remove waiting_thread from cpu waiting_set
        // TODO: how to track waiting thread with unique pointers?
        // In the current implementation, it seems there would be a u_ptr to the waiting thread
        // in BOTH the waiting_set and running_context (context_wrapper).

        // add waiting_thread to cpu ready_queue

    }

    // add current/running context to finished_queue
    finished_queue.push(std::move(cpu::self()->impl_ptr->running_context));

    // enable interrupts
    cpu::interrupt_enable();


    // TODO: implement cpu suspend on some condition here
    // when there are no threads available to be run
}

void thread::impl::impl_thread_yield() {
	// is this identical to impl_timer_interrupt_handler?
    // consider edge cases and abstracting this function
}