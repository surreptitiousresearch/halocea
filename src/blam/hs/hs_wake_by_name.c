/* hs_wake_by_name @0x8368E0D8 — find the running HaloScript thread whose scenario script matches
 * `script_name` (case-insensitive) and wake it. Returns 1 if a matching sleeping thread was woken,
 * 0 otherwise. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/hs_thread.h"
#include "headers/hs_script.h"
#include "headers/scenario.h"
#include "headers/blam_data_globals.h"


#include "headers/data_array.h"
extern int data_next_index(const data_array *data, int16_t index);
extern void hs_wake(int thread_index);
extern int  stricmp(const char *a, const char *b);

int hs_wake_by_name(const char *script_name)
{
    int thread_index;
    for (thread_index = data_next_index(hs_thread_data, -1);
         thread_index != -1;
         thread_index = data_next_index(hs_thread_data, thread_index))
    {
        hs_thread *thread = DATA_ARRAY_ELEMENT(hs_thread_data, hs_thread, thread_index);
        int script_index = thread->script_index;
        if (script_index != -1)
        {
            const char *script_name_stored =
                ((hs_script *)global_scenario->hs_scripts.address)[script_index].name;
            if (!stricmp(script_name_stored, script_name))
                break;
        }
    }

    if (thread_index != -1)
    {
        hs_wake(thread_index);
        return 1;
    }
    return 0;
}
