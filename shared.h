#ifndef SHARED_H
#define SHARED_H

#include <queue>
#include <unordered_set>
#include <memory>
#include <ucontext.h>

struct context_wrapper {
  std::queue<std::unique_ptr<context_wrapper>> waiting_contexts;
  ucontext_t* context_ptr;
  char* stack_base;
};

extern std::queue<std::unique_ptr<context_wrapper>> ready_queue;
// extern std::unordered_set<std::unique_ptr<context_wrapper>> waiting_set;
extern std::queue<std::unique_ptr<context_wrapper>> finished_queue;

#endif
