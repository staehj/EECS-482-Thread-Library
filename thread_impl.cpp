#include "thread_impl.h"

thread::impl::impl() {
    // add thread context to cpu ready_queue
}

thread::impl::~impl() {

}

// Note, if A calls B.join(), then BOTH A and B thread objects must exist
// So we do not have to worry about A or B thread objects being destroyed
void thread::impl::impl_join() {
    // set current(/calling) context as waiting_thread of called thread
        // eg. if A calls B.join --> A is calling thread, B is called thread
    // put calling context into cpu waiting_set
}