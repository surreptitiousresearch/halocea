/* encounter_spawn_actor @0x8370BE60 — place (spawn) one actor of a squad into its encounter, bumping the
 * encounter's and squad's live counts, decrementing the squad's remaining respawns, and rolling fresh
 * encounter- and squad-level respawn delay timers from the definition's delay ranges. Always returns 0. */

#include <stdint.h>
#include "headers/encounter_datum.h"
#include "headers/encounter_definition.h"
#include "headers/squad_definition.h"
#include "headers/data_array.h"
#include "headers/data_array.h"
#include "headers/scenario.h"
#include "headers/squad_datum.h"
#include "headers/ai_globals.h"
#include "headers/blam_data_globals.h"

extern uint8_t encounter_place_actor(int encounter_index, int16_t squad_index, int16_t variant_number, uint8_t spawn);
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random_range(uint32_t *seed, float lower_bound, float upper_bound);

uint8_t encounter_spawn_actor(uint16_t encounter_index, int16_t squad_index)
{
    if ( ai_globals->ai_initialized_for_map && encounter_place_actor(encounter_index, squad_index, 0, 1u) )
    {
        encounter_datum *encounter = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_index);
        encounter_definition *encounter_def = (encounter_definition *)global_scenario->ai_encounters.address + encounter_index;
        squad_definition *squad_def = (squad_definition *)encounter_def->squads.address + squad_index;
        squad_datum *squad = &squad_array[(int16_t)(encounter->squad_base + squad_index)];

        ++encounter->current_count;
        ++squad->current_count;
        if ( squad_def->respawn_total_count > 0 )
            --squad->respawn_actors_left;

        float encounter_delay_max = encounter_def->respawn_time_upper_bound;
        float encounter_delay_min = encounter_def->respawn_time_lower_bound;
        unsigned int *seed = get_global_random_seed_address();
        encounter->respawn_delay_ticks = (int)(real_seed_random_range(seed, encounter_delay_min, encounter_delay_max) * 30.0f);

        float squad_delay_max = squad_def->respawn_time_upper_bound;
        float squad_delay_min = squad_def->respawn_time_lower_bound;
        unsigned int *squad_seed = get_global_random_seed_address();
        squad->respawn_delay_ticks = (int)(real_seed_random_range(squad_seed, squad_delay_min, squad_delay_max) * 30.0f);
    }
    return 0;
}
