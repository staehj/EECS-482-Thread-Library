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

	// should this be static?
	static void thread_wrapper(thread_startfunc_t body, void* arg);

	// should this be static?
	static void thread_exit();

	// Function for handling timer interrupts
	void impl_timer_interrupt_handler();

	// Function for handling thread yield
	void impl_thread_yield();

	// Implementation for cpu::init
	void impl_init(thread_startfunc_t, void*);

private:
	// Add any private data members to implement this function
	std::queue<context_wrapper> ready_queue;
	std::unordered_set<context_wrapper> waiting_set;
	std::queue<context_wrapper> finished_queue;
	context_wrapper running_context;
};

#endif /* _CPU_IMPL_H */
