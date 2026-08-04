/* release_mutex @0x83800330 — release the underlying Win32 mutex object. */

#include "headers/mutex.h"
#include <windows.h>

void release_mutex(const mutex *mutex_reference)
{
    ReleaseMutex(mutex_reference->object);
}
