#define _XOPEN_SOURCE

#include "thread_impl.h"

#include <memory>
#include <ucontext.h>

#include "context_wrapper.h"

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