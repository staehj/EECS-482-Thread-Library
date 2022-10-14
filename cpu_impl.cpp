#include "cpu.h"
#include "cpu_impl.h"

#include <cassert>
#include <memory>
#include <queue>
#include <ucontext.h>

#include "shared.h"
#include "thread.h"
#include "thread_impl.h"


void cpu::impl::impl_timer_interrupt_handler() {
	// disable interrupts
    cpu::interrupt_disable();

    // shortcut for cpu::self()->impl_ptr
    impl* cpu_self = cpu::self()->impl_ptr;

    // if ready queue is empty, we can stay on the current thread
    if (!ready_queue.empty()) {
        // add running_context to ready_queue
        ucontext_t* temp = cpu_self->running_context->context_ptr;
        ready_queue.push(std::move(cpu_self->running_context));

        // retrive/pop context from ready_queue + assign as running_context
        cpu_self->running_context = std::move(ready_queue.front());
        ready_queue.pop();

        // save current context and start new context
        swapcontext(temp, cpu_self->running_context->context_ptr);
    }

    // enable interrupts
    cpu::interrupt_enable();
}

// Assumes interrupt vector is initialized
void cpu::impl::impl_init(thread_startfunc_t body, void* arg) {
    // initialize interrupt vector
    cpu::self()->interrupt_vector_table[TIMER] = impl_timer_interrupt_handler;

    // wrap thread_func with thread_wrapper
    void (*wrapped_func_ptr)(thread_startfunc_t, void*);
    wrapped_func_ptr = thread::impl::thread_wrapper;

    // make context with wrapper thread_func
    auto context = std::move(thread::impl::set_up_context());
    makecontext(context->context_ptr, (void (*)()) wrapped_func_ptr, 2, body, arg);

    // shortcut for cpu::self()->impl_ptr
    impl* cpu_self = cpu::self()->impl_ptr;

    // assign context as running_context
    cpu_self->running_context = std::move(context);

    // set/start context
    setcontext(cpu_self->running_context->context_ptr);
}
