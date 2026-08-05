/* encounter_update_squads @ 0x8370C300 — per-tick squad maintenance for one encounter. For each squad in
 * the encounter (squad_base..squad_base+squad_count) whose spawn delay timer is running and whose scenario
 * squad definition is not flagged "no spawn" (def flag bit 0x8): if the timer has already started, age it by
 * 15 ticks and expire it when it reaches the window; otherwise start the timer once the squad's spawn
 * condition holds (def flag bit 0x4 set, or the encounter already has units in combat).
 *
 * The scenario definitions are resolved to the DB encounter_definition (176 bytes,
 * squads.address @+132) and squad_definition (232 bytes, flags @+40) structs. */

#include <stdint.h>
#include "headers/squad_definition.h"
#include "headers/squad_flags.h"
#include "headers/encounter_definition.h"
#include "headers/encounter_datum.h"
#include "headers/squad_datum.h"
#include "headers/scenario.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"


extern void encounter_squad_timer_expire(uint16_t encounter_index, int16_t squad_index);

void encounter_update_squads(int encounter_index)
{
    encounter_datum *encounter = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_index);
    encounter_definition *encounter_def =
        &((encounter_definition *)global_scenario->ai_encounters.address)[(uint16_t)encounter_index];

    if ( encounter->squad_count <= 0 )
        return;

    squad_definition *squad_definitions = (squad_definition *)encounter_def->squads.address;
    for ( int16_t squad_local_index = 0; squad_local_index < encounter->squad_count; ++squad_local_index )
    {
        squad_datum *squad = &squad_array[(int16_t)(encounter->squad_base + squad_local_index)];
        int16_t delay_timer = squad->delay_timer;
        if ( delay_timer <= 0 )
            continue;

        int squad_flags = squad_definitions[squad_local_index].flags;
        if ( (squad_flags & (1u << _squad_delay_forever_bit)) != 0 )
            continue;

        if ( squad->delay_timer_started )
        {
            if ( delay_timer <= 15 )
                encounter_squad_timer_expire(encounter_index, squad_local_index);
            else
                squad->delay_timer = delay_timer - 15;
        }
        else
        {
            squad->delay_timer_started =
                ((squad_flags & (1u << _squad_timer_starts_immediately_bit)) != 0
                    || encounter->current_in_combat_count > 0) ? 1 : 0;
        }
    }
}
