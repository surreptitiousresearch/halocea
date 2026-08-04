/* encounter_update_respawn @ 0x8370C020 — drive an encounter's squad respawning. Once the respawn timer
 * elapses (aged in 15-tick steps), walk every squad: top each squad back up to its minimum live count by
 * spawning immediately, and for squads still below their normal strength count down a per-squad respawn
 * delay; squads whose delay just expired are recorded in a bitmask. Finally, if any squad came due this
 * pass, pick one at random among them and spawn a single actor into it.
 *
 * The scenario definitions are resolved to the DB encounter_definition (176 bytes, squads.count @+128,
 * squads.address @+132) and squad_definition (232 bytes, respawn_min_actors @+132,
 * respawn_max_actors @+134) structs.
 *
 * Deviation: the decompiler rendered the bitmask init `std r23,0(r8)` (a 64-bit zero) as
 * `v15[0] = 0x100000000LL`; disasm at 0x8370C09C confirms it stores 0. The mask is a 64-bit (two-dword)
 * field, indexed by squad-local index. */

#include <stdint.h>
#include "headers/squad_definition.h"
#include "headers/encounter_datum.h"
#include "headers/encounter_definition.h"
#include "headers/squad_datum.h"
#include "headers/scenario.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"
#include "headers/bit_vector.h"


extern uint8_t encounter_spawn_actor(uint16_t encounter_index, int16_t squad_index);
extern uint32_t *get_global_random_seed_address(void);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);

void encounter_update_respawn(int encounter_index)
{
    encounter_datum *encounter = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_index);

    if ( !encounter->respawn_enabled )            /* recovered: encounter[0x3C] */
        return;

    __int16 respawn_timer = encounter->respawn_delay_ticks; /* recovered: *(__int16 *)(encounter + 0x3E) */
    if ( respawn_timer > 15 )
    {
        encounter->respawn_delay_ticks = respawn_timer - 15;
        return;
    }

    encounter_definition *encounter_def =
        &((encounter_definition *)global_scenario->ai_encounters.address)[(unsigned __int16)encounter_index];
    encounter->respawn_delay_ticks = 0;           /* recovered: *(__int16 *)(encounter + 0x3E) */

    unsigned int respawn_window_mask[2] = { 0, 0 };
    __int16 due_count = 0;

    int squad_def_count = encounter_def->squads.count;
    for ( __int16 squad_local_index = 0; squad_local_index < squad_def_count; ++squad_local_index )
    {
        squad_definition *squad_def = &((squad_definition *)encounter_def->squads.address)[squad_local_index];
        squad_datum *squad = &squad_array[(__int16)(squad_local_index + encounter->squad_base)]; /* recovered: *(__int16 *)(encounter + 4) */

        if ( squad->respawn_actors_left > 0 )
        {
            while ( squad->current_count < squad_def->respawn_min_actors
                 && encounter_spawn_actor(encounter_index, squad_local_index)
                 && squad->respawn_actors_left > 0 )
                ;
        }

        if ( squad->respawn_actors_left > 0 && squad->current_count < squad_def->respawn_max_actors )
        {
            __int16 respawn_delay_ticks = squad->respawn_delay_ticks;
            if ( respawn_delay_ticks <= 15 )
            {
                squad->respawn_delay_ticks = 0;
                ++due_count;
                BIT_VECTOR_SET_FLAG(respawn_window_mask, squad_local_index);
            }
            else
            {
                squad->respawn_delay_ticks = respawn_delay_ticks - 15;
            }
        }
    }

    if ( due_count > 0 && !encounter->respawn_delay_ticks )  /* recovered: *(__int16 *)(encounter + 0x3E) */
    {
        __int16 pick = seed_random_range(get_global_random_seed_address(), 0, due_count);
        for ( __int16 squad_local_index = 0;
              squad_local_index < encounter->squad_count;            /* recovered: *(__int16 *)(encounter + 6) */
              ++squad_local_index )
        {
            if ( BIT_VECTOR_TEST_FLAG(respawn_window_mask, squad_local_index) )
            {
                if ( pick <= 0 )
                {
                    if ( encounter_spawn_actor(encounter_index, squad_local_index) )
                        return;
                }
                else
                {
                    --pick;
                }
            }
        }
    }
}
