#include "cpu.h"
#include "cpu_impl.h"

#include <ucontext.h>

// Initialize global variables
std::queue<std::unique_ptr<context_wrapper>> ready_queue;
std::queue<std::unique_ptr<context_wrapper>> finished_queue;
std::vector<thread::impl*> thread_vec;
int unique_id = 0;

void cpu::init(thread_startfunc_t body, void* arg) {
    impl_ptr->impl_init(body, arg);
}
