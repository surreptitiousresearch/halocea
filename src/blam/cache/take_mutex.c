/* take_mutex @0x838002D0 — wait on the underlying Win32 mutex with a timeout. Returns 1 if the mutex was
 * acquired (WAIT_OBJECT_0) or abandoned (WAIT_ABANDONED, 128); 0 on timeout. */

#include <stdint.h>
#include "headers/mutex.h"
#include <windows.h>

uint8_t take_mutex(const mutex *mutex_reference, unsigned int milliseconds_to_wait)
{
    unsigned int result = WaitForSingleObject(mutex_reference->object, milliseconds_to_wait);
    return result == WAIT_OBJECT_0 || result == WAIT_ABANDONED; /* WAIT_ABANDONED == 128 */
}
