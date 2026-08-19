#include "headers/blam_data_globals.h"
/* debug_keys_dispose @ 0x836EAC50 — free the debug-key down-state bitset */

extern void dlFree(void *ptr);

void debug_keys_dispose(void)
{
    if ( global_debug_key_down )
    {
        dlFree(global_debug_key_down);
        global_debug_key_down = 0;
    }
}
