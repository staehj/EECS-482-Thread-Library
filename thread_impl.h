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

	void impl_join();

private:
	std::shared_ptr<ucontext_t> waiting_thread;
};

#endif /* _THREAD_IMPL_H */
