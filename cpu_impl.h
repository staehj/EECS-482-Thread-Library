#ifndef _CPU_IMPL_H
#define _CPU_IMPL_H
#define _XOPEN_SOURCE

#include "cpu.h"

#include <memory>
#include <queue>
#include <unordered_set>
#include <ucontext.h>

#include "context_wrapper.h"

class cpu::impl{
public:
	impl();

	~impl();

	// Wrap thread with thread_exit to track end of stream of execution
	static void thread_wrapper(thread_startfunc_t body, void* arg);

	// Detect end of stream of execution
	void thread_exit();

	// Handle timer interrupt
	void impl_timer_interrupt_handler();

	// Handle thread yield
	void impl_thread_yield();

	// Implementation for cpu::init
	void impl_init(thread_startfunc_t, void*);

private:
	// Add any private data members to implement this function
	std::queue<std::unique_ptr<context_wrapper>> ready_queue;
	std::unordered_set<std::unique_ptr<context_wrapper>> waiting_set;
	std::queue<std::unique_ptr<context_wrapper>> finished_queue;
	std::unique_ptr<context_wrapper> running_context;
};

#endif /* _CPU_IMPL_H */
