/* actor_action_allowed_to_enter_vehicle @0x837F23B0 — true unless the actor was told to avoid entering this
 * exact vehicle and the avoidance window (until word +229, a game-time tick) hasn't expired yet.
 * DEVIATION: __CFADD__ carry idiom is the signed test game_time_get() >= avoid_until (numerically verified
 * !__CFADD__(a-b,b^0x80000000) == (a>=b); see actor_look_secondary.c for the same idiom, corrected there). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/blam_data_globals.h"

extern int game_time_get(void);

uint8_t actor_action_allowed_to_enter_vehicle(int actor_index, int vehicle_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    return vehicle_index != actor->emotions.vehicle_ignore_index || game_time_get() >= actor->emotions.vehicle_ignore_time;
}
