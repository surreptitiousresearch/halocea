/* actor_stimulus_clear @ 0x837D3C68 — zero the actor's per-tick stimulus scratch block (actor+748, 0x64
 * bytes), called once per arbitration round in actor_update. */

#include <string.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"


void actor_stimulus_clear(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    memset(&actor->stimuli, 0, sizeof(actor->stimuli));
}
