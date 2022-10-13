#ifndef _THREAD_IMPL_H
#define _THREAD_IMPL_H
#define _XOPEN_SOURCE

#include "thread.h"

#include <memory>
#include <queue>
#include <ucontext.h>
#include "shared.h"

class thread::impl {
public:
	impl(thread_startfunc_t thread_func, void* param);

	~impl();

	static std::unique_ptr<context_wrapper> set_up_context();

	void impl_join();

	static void thread_exit();

	static void thread_wrapper(thread_startfunc_t body, void* arg);

	static void impl_thread_yield();

private:
  std::queue<std::unique_ptr<context_wrapper>> waiting_queue;
	int id;
};

#endif /* _THREAD_IMPL_H */
