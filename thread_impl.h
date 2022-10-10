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

	static context_wrapper set_up_context();

	void impl_join();

private:
	std::unique_ptr<ucontext_t> waiting_thread;
};

#endif /* _THREAD_IMPL_H */
