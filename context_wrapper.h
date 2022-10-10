
#include <memory>
#include <ucontext.h>

struct context_wrapper {
  ucontext_t context_ptr;
  char* stack_base;
};