/* dispose_thread @ 0x838001A0 — close a thread's Win32 handle and mark its slot free. */

#include "headers/thread.h"

extern int CloseHandle(void *handle);

void dispose_thread(thread *thread_reference)
{
    CloseHandle(thread_reference->object);
    thread_reference->object = 0;
    thread_reference->in_use = 0;
}
