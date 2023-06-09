#ifndef _CPU_IMPL_H
#define _CPU_IMPL_H

#include "cpu.h"

#include <cassert>
#include <memory>
#include <queue>
#include <unordered_set>
#include <ucontext.h>

#include "shared.h"


class cpu::impl{
public:
	// Handle timer interrupt
	static void impl_timer_interrupt_handler();

	// Implementation for cpu::init
	void impl_init(thread_startfunc_t, void*);

	std::unique_ptr<context_wrapper> running_context;

private:
	// Add any private data members to implement this function
};

#endif /* _CPU_IMPL_H */
