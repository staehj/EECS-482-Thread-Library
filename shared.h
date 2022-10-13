#ifndef SHARED_H
#define SHARED_H

#include <queue>
#include <unordered_set>
#include <memory>
#include <ucontext.h>

struct context_wrapper {
  ucontext_t* context_ptr;
  char* stack_base;
  int id;

  context_wrapper() : id(unique_id) {
    unique_id++;
  }
};

extern std::queue<std::unique_ptr<context_wrapper>> ready_queue;
extern std::queue<std::unique_ptr<context_wrapper>> finished_queue;
extern int unique_id;

#endif
