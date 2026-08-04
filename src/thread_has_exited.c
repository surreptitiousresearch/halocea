/* thread_has_exited @ 0x83800150 — true once the Win32 thread is no longer STILL_ACTIVE. */

#include <stdint.h>
#include "headers/thread.h"

extern int GetExitCodeThread(void *handle, unsigned int *exit_code);

uint8_t thread_has_exited(const thread *thread_reference)
{
    unsigned int exit_code;

    if ( !GetExitCodeThread(thread_reference->object, &exit_code) )
        return 0;
    if ( exit_code == 259 )   /* STILL_ACTIVE */
        return 0;
    return 1;
}
