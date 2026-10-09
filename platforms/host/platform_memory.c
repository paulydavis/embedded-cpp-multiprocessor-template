#include "project/platform/platform_memory.h"

void project_memory_barrier(void) { __atomic_thread_fence(__ATOMIC_SEQ_CST); }
