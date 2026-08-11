/* dispose_mutex @0x83800338 — release a named mutex: close its Win32 handle and clear the record. */

#include "headers/mutex.h"

extern int CloseHandle(void *handle);

void dispose_mutex(mutex *mutex_reference)
{
    CloseHandle(mutex_reference->object);
    mutex_reference->name[0] = 0;
    mutex_reference->object = 0;
    mutex_reference->in_use = 0;
}
