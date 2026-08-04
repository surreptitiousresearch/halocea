/* actors_update @ 0x83721CD0 — the per-frame actor pump. Resets the service-timer budget for this frame
 * (carries last frame's high-water mark forward as the new baseline), then walks every live actor: those
 * flagged for erasure are erased, the rest are updated if their mode is active (>0). */

#include <stdint.h>
#include "headers/ai_globals.h"
#include "headers/actor_iterator.h"
#include "headers/actor_datum.h"
#include "headers/actor_mode.h"

extern void actor_erase(int actor_index, uint8_t immediate);
extern void actor_update(int actor_index);

void actors_update(void)
{
    actor_iterator iterator;
    actor_datum   *actor;

    ai_globals->last_highest_service_timer = ai_globals->current_highest_service_timer;
    ai_globals->current_highest_service_timer = 0;
    ai_globals->time_given_this_frame = 0;

    actor_iterator_new(&iterator, 1u);
    for ( actor = actor_iterator_next(&iterator); actor; actor = actor_iterator_next(&iterator) )
    {
        if ( actor->meta.erase_next_tick )
            actor_erase(iterator.index, 0);
        else if ( actor->state.mode > _actor_mode_braindead )
            actor_update(iterator.index);
    }
}
