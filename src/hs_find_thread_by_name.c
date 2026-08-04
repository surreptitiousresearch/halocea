/* hs_find_thread_by_name @0x8368D450 — find the running script thread whose bound script's name
 * matches `name` (case-insensitive). Walks the hs_thread data array; each thread's script_index
 * (0 for none is -1 here) indexes the scenario's hs_scripts block. Returns the thread's datum index
 * or -1 if none matches. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/hs_thread.h"
#include "headers/hs_script.h"
#include "headers/scenario.h"

#include "headers/data_array.h"
extern data_array *hs_thread_data;
extern scenario   *global_scenario;

extern int data_next_index(const data_array *data, int16_t index);
extern int  stricmp(const char *a, const char *b);

int hs_find_thread_by_name(const char *name)
{
    int index = data_next_index(hs_thread_data, -1);

    while ( index != -1 )
    {
        int script_index = DATA_ARRAY_ELEMENT(hs_thread_data, hs_thread, index)->script_index;
        if ( script_index != -1 )
        {
            const hs_script *script =
                &((const hs_script *)global_scenario->hs_scripts.address)[script_index];
            if ( !stricmp(script->name, name) )
                return index;
        }
        index = data_next_index(hs_thread_data, index);
    }
    return -1;
}
