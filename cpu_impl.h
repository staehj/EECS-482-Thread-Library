#ifndef _CPU_IMPL_H
#define _CPU_IMPL_H

#include <queue>
#include <ucontext.h>

#include "cpu.h"

class cpu::impl{
public:
	impl();

	~impl();

	// Function for handling timer interrupts
	void impl_timer_interrupt_handler();

	// Function for handling thread yield
	void impl_thread_yield();

	// Implementation for cpu::init
	void impl_init(thread_startfunc_t, void*);

private:
	// Add any private data members to implement this function
    std::queue<ucontext_t*> ready_queue;
};

#endif /* _CPU_IMPL_H */
