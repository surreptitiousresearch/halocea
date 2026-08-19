/* actor_stimulus_suspicion @0x837D4068 — raises the actor's suspicion-level combat status (word +842) and
 * timer (dword +844) if the new status is stronger; if it's the same status, extends the timer to the max
 * of the current and new value; a weaker status is ignored entirely. Fields resolved to the DB-named
 * actor_datum members stimuli.suspicion_combat_status / stimuli.suspicion_timer. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"


void actor_stimulus_suspicion(int actor_index, int16_t suspicion_combat_status, int suspicion_timer)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int16_t current_status = actor->stimuli.suspicion_combat_status;

    if (current_status < suspicion_combat_status)
    {
        actor->stimuli.suspicion_combat_status = suspicion_combat_status;
        actor->stimuli.suspicion_timer = suspicion_timer;
    }
    else if (current_status == suspicion_combat_status)
    {
        int *timer = &actor->stimuli.suspicion_timer;
        if (*timer <= suspicion_timer)
            *timer = suspicion_timer;
    }
}
