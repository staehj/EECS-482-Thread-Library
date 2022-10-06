#include "thread_impl.h"

// Wrapper for thread function to track completion of stream of execution
void thread_wrapper(/* thread_function */) {
    // call thread_function
    // call thread_exit
}

// Called after completion of stream of execution
void thread_exit() {
    // disable interrupts
    // clear cpu finished_queue (no need to delete explicitly due to smart pointers)
    // add current context to finished_queue
    // if waiting_thread exists for this thread:
        // remove waiting_thread from cpu waiting_set
        // add waiting_thread to cpu ready_queue
    // enable interrupts
}

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