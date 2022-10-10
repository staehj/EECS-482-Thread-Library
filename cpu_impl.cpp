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
    // TODO: figure out static stuff
    // in particular, we are inside a static function (which must be static
    // for makecontext purposes) and want to call a non-static function
    // does this even conceptually make sense? what is a better design?
    thread_exit();
}

// Called after completion of stream of execution
void cpu::impl::thread_exit() {
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

void cpu::impl::impl_timer_interrupt_handler() {
	// disable interrupts
    cpu::interrupt_disable();

    // if ready queue is empty, we can stay on the current thread
    if (!ready_queue.empty()) {
        // add running_context to ready_queue
        ready_queue.push(std::move(running_context));

        // retrive/pop context from ready_queue + assign as running_context
        running_context = std::move(ready_queue.front());
        ready_queue.pop();

        // set/start popped context
        setcontext(running_context->context_ptr);
    }

    // enable interrupts
    cpu::interrupt_enable();
}

void cpu::impl::impl_thread_yield() {
	// is this identical to impl_timer_interrupt_handler?
    // consider edge cases and abstracting this function
}

// Assumes interrupt vector is initialized
void cpu::impl::impl_init(thread_startfunc_t body, void* arg) {
    // TODO: initialize interrupt vector
    // interrupt_handler_t handler_func_ptr = (void(*)()) impl_timer_interrupt_handler;
    cpu::self()->interrupt_vector_table[TIMER] = (void(*)()) impl_timer_interrupt_handler;

    // wrap thread_func with thread_wrapper
    void (*wrapped_func_ptr)(thread_startfunc_t, void*);
    wrapped_func_ptr = thread_wrapper;

    // make context with wrapper thread_func
    auto context = std::move(thread::impl::set_up_context());
    makecontext(context->context_ptr, (void (*)()) wrapped_func_ptr, 2, body, arg);

    // assign context as running_context
    running_context = std::move(context);  // TODO: why this no work? :(
                                           // must we overload std::move?
                                           // BUT this works if I change context with running_context
                                           // what is going on...?

    // enable interrupts
    cpu::interrupt_enable();

    // set/start context
    setcontext(running_context->context_ptr);
}
