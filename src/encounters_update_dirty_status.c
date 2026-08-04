/* encounters_update_dirty_status @ 0x8370DC50 — once-a-second pass (driven from encounters_update): walk
 * every encounter and recompute the perception status of any whose status_dirty flag is set.
 * Return type is void: the r3 at blr is only the loop-exit (null) iterator residue and no caller
 * consumes it (the decompiler's `return result` is an artifact of that residue). */

#include "headers/ai_globals.h"
#include "headers/encounter_iterator.h"
#include "headers/encounter_datum.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern void encounter_update_status(int encounter_index);

void encounters_update_dirty_status(void)
{
    encounter_iterator iterator;
    encounter_datum   *result;

    if ( ai_globals->ai_initialized_for_map )
    {
        data_iterator_new(&iterator.encounter_iterator, encounter_data);
        iterator.active_only = 0;
    }

    for ( result = encounter_iterator_next(&iterator); result; result = encounter_iterator_next(&iterator) )
    {
        if ( result->status_dirty )
            encounter_update_status(iterator.index);
    }
}
