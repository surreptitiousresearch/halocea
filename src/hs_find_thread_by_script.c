/* hs_find_thread_by_script @0x8368D3C8 — return the index of the live HS thread running the given script, or
 * -1. Each thread datum is 536 bytes; the script index it runs is the dword at offset 4. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/hs_thread.h"
#include "headers/blam_data_globals.h"

extern int data_next_index(const data_array *data, int16_t index);

int hs_find_thread_by_script(int16_t script_index)
{
    int index = data_next_index(hs_thread_data, -1);
    if ( index == -1 )
        return -1;

    /* recovered: (int *)hs_thread_data->data + 134*idx + 1 -> hs_thread.script_index
     * (134*4 = 536 = sizeof(hs_thread), +1*4 = +4). */
    while ( DATA_ARRAY_ELEMENT(hs_thread_data, hs_thread, index)->script_index != script_index )
    {
        index = data_next_index(hs_thread_data, index);
        if ( index == -1 )
            return -1;
    }
    return index;
}
