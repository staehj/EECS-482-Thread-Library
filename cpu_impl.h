#ifndef _CPU_IMPL_H
#define _CPU_IMPL_H

#include "cpu.h"

#include <memory>
#include <queue>
#include <unordered_set>
#include <ucontext.h>

class cpu::impl{
public:
	impl();

	~impl();

    // should this be static?
    void thread_wrapper(/* thread_function, args */);

    // should this be static?
    void thread_exit();

	// Function for handling timer interrupts
	void impl_timer_interrupt_handler();

	// Function for handling thread yield
	void impl_thread_yield();

	// Implementation for cpu::init
	void impl_init(thread_startfunc_t, void*);

private:
	// Add any private data members to implement this function
    std::queue<std::shared_ptr<ucontext_t>> ready_queue;
    std::unordered_set<std::shared_ptr<ucontext_t>> waiting_set;
    std::queue<std::shared_ptr<ucontext_t>> finished_queue;
    std::shared_ptr<ucontext_t> running_context;
};

#endif /* _CPU_IMPL_H */
