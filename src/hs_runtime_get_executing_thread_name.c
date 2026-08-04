/* hs_runtime_get_executing_thread_name @0x8368E090 — human-readable label for the HaloScript thread
 * currently executing, or "[unknown]" when none is set or the thread has no name. */

#include <stdint.h>
#include "headers/hs_runtime_globals.h"

extern const char * hs_thread_format(uint16_t thread_index);

const char * hs_runtime_get_executing_thread_name(void)
{
    if ( hs_runtime_globals.executing_thread_index == -1 )
        return "[unknown]";

    const char *name = hs_thread_format(hs_runtime_globals.executing_thread_index);
    if ( !name )
        return "[unknown]";
    return name;
}
