/* encounter_mark_examined_pursuit_position @0x8370D270 — record that `actor_index` has examined a firing
 * position during a pursuit (creating the pursuit record if needed via encounter_find_pursuit). Pursuit
 * record fields (40-byte stride, per encounter_pursuit_position_already_examined.c): last-examined time
 * dword +4, examiner count word +8, examiner actor indices dwords +12..+32 (6 slots, written round-robin
 * via a write cursor word +10 not previously named). If the actor is already among the 6 recorded
 * examiners, only the last-examined time is refreshed; otherwise it's inserted at the cursor slot, the
 * count is incremented (unbounded — encounter_pursuit_position_already_examined.c's "count >= 7" fast path
 * relies on this), and the cursor advances mod 6. Returns 1 if this call newly recorded the actor, 0 if it
 * was already present (or no pursuit could be found/created). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/pursuit_datum.h"
#include "headers/blam_data_globals.h"

extern int game_time_get(void);

extern int encounter_find_pursuit(uint16_t encounter_index, int16_t firing_position_index, int history_start_time, uint8_t force_create);

uint8_t encounter_mark_examined_pursuit_position(int encounter_index, int actor_index, int16_t firing_position_index, int history_start_time)
{
    int newly_recorded = 0;

    int pursuit_index = encounter_find_pursuit(encounter_index, firing_position_index, history_start_time, 1u);
    if ( pursuit_index != -1 )
    {
        pursuit_datum *pursuit = DATA_ARRAY_ELEMENT(pursuit_data, pursuit_datum, pursuit_index);

        __int16 examiner_slot;
        for ( examiner_slot = 0; examiner_slot < 6; examiner_slot++ )
        {
            if ( pursuit->actor_indices[examiner_slot] == actor_index )
                break;
        }

        if ( examiner_slot >= 6 )
        {
            newly_recorded = 1;
            __int16 next_examiner_slot = pursuit->next_actor_index_index;
            pursuit->actor_indices[next_examiner_slot] = actor_index;
            ++pursuit->actor_count;
            pursuit->next_actor_index_index = (next_examiner_slot + 1) % 6;
        }

        pursuit->last_examined_time = game_time_get();
    }

    return newly_recorded;
}
