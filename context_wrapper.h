
#include <memory>
#include <ucontext.h>

struct context_wrapper {
  // std::unique_ptr<context_wrapper> waiting_context;
  ucontext_t* context_ptr;
  char* stack_base;
};
