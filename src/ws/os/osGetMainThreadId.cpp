#include "../../headers/ws/os/os_boundary.h"

// Returns the thread id captured for the process's main thread at startup.
int osGetMainThreadId()
{
    return MAIN_THREAD_ID;
}
