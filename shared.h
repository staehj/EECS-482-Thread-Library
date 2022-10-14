#ifndef SHARED_H
#define SHARED_H

#include <queue>
#include <unordered_set>
#include <memory>
#include <ucontext.h>

#include "thread.h"

struct context_wrapper {
  ucontext_t* context_ptr;
  char* stack_base;
  int id;

  context_wrapper() : context_ptr(nullptr), stack_base(nullptr), id(0) {}

  context_wrapper(int input_id):  id(input_id) {}
};

extern std::queue<std::unique_ptr<context_wrapper>> ready_queue;
extern std::queue<std::unique_ptr<context_wrapper>> finished_queue;
extern std::vector<thread::impl*> thread_vec;
extern int unique_id;

#endif
