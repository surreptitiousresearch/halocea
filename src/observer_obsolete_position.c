/* observer_obsolete_position @0x8370F5E8 — mark a local player's observer (camera) position as obsolete by
 * clearing its observer record. Tail-calls observer_clear on the per-local-player record within
 * observer_globals (float dtime header at 0x00, then local_players[2] at stride 0x29C = sizeof(observer)). */

#include <stdint.h>
#include "headers/observer_globals.h"

extern void observer_clear(observer *observer);

void observer_obsolete_position(int16_t local_player_index)
{
    /* recovered: (observer *)(observer_globals + 0x29C * index + 4) -> observer_globals.local_players[index] */
    observer_clear(&observer_globals.local_players[local_player_index]);
}
