#ifndef _THREAD_IMPL_H
#define _THREAD_IMPL_H
#define _XOPEN_SOURCE

#include "thread.h"

#include <memory>
#include <ucontext.h>

class thread::impl {
public:
	impl();

	~impl();

	static std::unique_ptr<context_wrapper> set_up_context();

	void impl_join();

	static void thread_exit();

	static void thread_wrapper(thread_startfunc_t body, void* arg);

	static void impl_thread_yield();

private:
  // Note: can't track this here because thread object may be
	// out of scope when stream of execution completes.
	// Instead, we could track this in context_wrapper
	// std::unique_ptr<ucontext_t> waiting_thread;
};

#endif /* _THREAD_IMPL_H */
