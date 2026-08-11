/* hs_thread_format @0x8368D098 — return a human-readable label for a HaloScript thread. Script threads
 * (type 0) are named by their scenario script; the special thread types get fixed labels; anything else
 * yields NULL. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/hs_thread.h"
#include "headers/hs_script.h"
#include "headers/scenario.h"
#include "headers/blam_data_globals.h"


const char * hs_thread_format(int thread_index)
{
    hs_thread *thread = DATA_ARRAY_ELEMENT(hs_thread_data, hs_thread, thread_index);

    unsigned int type = thread->type;
    if (!thread->type)
        return ((hs_script *)global_scenario->hs_scripts.address)[thread->script_index].name;
    if (type == 1)
        return "[global initialize]";
    if (type < 3)
        return "[console command]";
    return nullptr;
}
