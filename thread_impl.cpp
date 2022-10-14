#include "thread_impl.h"

#include <cassert>
#include <memory>
#include <ucontext.h>

#include "cpu.h"
#include "cpu_impl.h"
#include "shared.h"


// ------------------------------
// ------------------------------
// TODO: write asserts everywhere
// ------------------------------
// ------------------------------

thread::impl::impl(thread_startfunc_t thread_func, void* param) {
    // disable interrupts
    cpu::interrupt_disable();
    
    // wrap thread_func with thread_wrapper
    void (*wrapped_func_ptr)(thread_startfunc_t, void*);
    wrapped_func_ptr = thread::impl::thread_wrapper;

    // make context with wrapper thread_func
    auto context = std::move(thread::impl::set_up_context());
    id = unique_id;
    thread_vec.push_back(this);
    unique_id++;
    makecontext(context->context_ptr, (void (*)()) wrapped_func_ptr, 2, thread_func, param);

    // add context to ready queue
    ready_queue.push(std::move(context));

    // enable interrupts
    cpu::interrupt_enable();
}

thread::impl::~impl() {
    // ensure no thread attempts to join to this thread after it is deleted
    assert(waiting_queue.empty());

}

std::unique_ptr<context_wrapper> thread::impl::set_up_context() {
    auto wrapper = std::make_unique<context_wrapper>(unique_id);
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
// A (context_wrapper) === cpu::self()->running_context
// B (thread_impl)     === this
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
    // enable interrupts (about to go to user code)
    cpu::interrupt_enable();
    
    // call thread_function
    (*body)(arg);

    // disable interrupts (re-entering OS code)
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

        // Note: no need to delete context object because it is a smart pointer
    }

    thread::impl* cur_thread = thread_vec[cpu::self()->impl_ptr->running_context->id];

    // ensure waiting threads (due to join) are moved to ready queue
    while (!cur_thread->waiting_queue.empty()) {
        // remove waiting_thread from waiting_queue
        auto context = std::move(cur_thread->waiting_queue.front());
        cur_thread->waiting_queue.pop();

        // add waiting_thread to cpu ready_queue
        ready_queue.push(std::move(context));
    }

    // add current/running context to finished_queue
    finished_queue.push(std::move(cpu::self()->impl_ptr->running_context));

    // suspend cpu there are no more threads to run
    if (ready_queue.empty()) {
        cpu::interrupt_enable_suspend();
    }

    // assign next ready as running
    auto context = std::move(ready_queue.front());
    ready_queue.pop();
    cpu::self()->impl_ptr->running_context = std::move(context);

    // set/start running_context
    setcontext(cpu::self()->impl_ptr->running_context->context_ptr);
}

void thread::impl::impl_thread_yield() {
	// copy + paste code for interrupt_handler here
}
