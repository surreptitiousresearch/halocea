/* encounters_update @ 0x8370E4D8 — the per-frame encounter pump. Once a second (game time % 30) it
 * refreshes dirty status and tests activation. Then, spreading work across a 15-tick window keyed by the
 * encounter index, it updates each active encounter: refreshes its perception status, ages the
 * enemy-visible / enemy-alive timers (reset to 0 while the condition holds, else advance by 15, capped at
 * the -1 sentinel), counts down the post-combat delay timer, and runs respawn / squad / platoon / follow /
 * actor-control passes. */

#include <stdint.h>
#include "headers/ai_globals.h"
#include "headers/encounter_iterator.h"
#include "headers/encounter_datum.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"

extern int game_time_get(void);
extern void encounters_update_dirty_status(void);
extern void encounters_test_activation(void);
extern void encounter_update_status(int encounter_index);
extern void encounter_update_respawn(int encounter_index);
extern void encounter_update_squads(int encounter_index);
extern void encounter_update_platoons(int encounter_index);
extern void encounter_update_follow(int encounter_index);
extern void encounter_control_actors(int encounter_index);

extern void encounter_update_timers(uint16_t encounter_index);
void encounters_update(void)
{
    encounter_iterator iterator;
    int                time = game_time_get();

    if ( !(time % 30) )
    {
        encounters_update_dirty_status();
        encounters_test_activation();
    }

    int16_t phase = time % 15;
    if ( ai_globals->ai_initialized_for_map )
    {
        data_iterator_new(&iterator.encounter_iterator, encounter_data);
        iterator.active_only = 1;
    }

    while ( encounter_iterator_next(&iterator) )
    {
        if ( (int16_t)iterator.index % 15 != phase )
            continue;

        encounter_update_status(iterator.index);
        /* DEVIATION: collapsed inlined copy of encounter_update_timers @0x8370BC60/0x8370BC78 (zero-xref donor) back into a call; the local `encounter_datum *encounter` fetch and the three timer if-blocks below it were the compiler's inlined donor body, verified field-for-field (offsets 68/69/71/72/74/80/84) against the donor's own decompile. */
        encounter_update_timers(iterator.index);

        encounter_update_respawn(iterator.index);
        encounter_update_squads(iterator.index);
        encounter_update_platoons(iterator.index);
        encounter_update_follow(iterator.index);
        encounter_control_actors(iterator.index);
    }
}
