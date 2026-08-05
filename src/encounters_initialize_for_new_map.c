/* encounters_initialize_for_new_map @0x8370CE0C — build the runtime AI encounter table from the scenario:
 * allocate an encounter datum per scenario encounter, copy its perception/team setup, then lay out its squads
 * (delay timers, migration, respawn, starting locations) and platoons (defending flag) into the global squad
 * and platoon arrays, assigning each encounter a contiguous squad/platoon base range.
 *
 * The encounter-datum fields at 0x38/0x3C/0x3E/0x5C resolve to the DB-named first_pursuit_index /
 * respawn_enabled / respawn_delay_ticks / last_grenade_throw_time members; scenario encounter/squad
 * definitions are read by offset (176-byte encounter stride; squads at def dword 33, 232-byte squad stride;
 * platoons at def dword 36, 172-byte platoon stride). */

#include <stdint.h>
#include "headers/encounter_definition.h"
#include "headers/platoon_flags.h"
#include "headers/encounter_flags.h"
#include "headers/squad_flags.h"
#include "headers/squad_definition.h"
#include "headers/platoon_definition.h"
#include "headers/scenario.h"
#include "headers/encounter_datum.h"
#include "headers/squad_datum.h"
#include "headers/platoon_datum.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"
extern void *memset(void *dst, int value, unsigned int n);


extern void data_make_valid(data_array *data);
extern int datum_new(data_array *data);
extern void squad_reset_starting_locations(uint16_t encounter_index, int16_t squad_index);

void encounters_initialize_for_new_map(void)
{
    int16_t squad_base_counter = 0;
    int16_t platoon_base_counter = 0;
    scenario *scenario_ptr = global_scenario;

    data_make_valid(encounter_data);
    data_make_valid(pursuit_data);
    memset(squad_array, 0, 0x8000u);
    memset(platoon_array, 0, 0x1000u);

    for ( int i = 0; i < scenario_ptr->ai_encounters.count; i = (int16_t)(i + 1) )
    {
        encounter_definition *encounter_def = (encounter_definition *)scenario_ptr->ai_encounters.address + i;
        int datum_index = datum_new(encounter_data);
        if ( datum_index == -1 )
            continue;

        encounter_datum *encounter = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, datum_index);
        unsigned int flags = encounter_def->flags;

        encounter->team_index = encounter_def->team_index;
        encounter->first_actor_index = -1;
        encounter->first_pursuit_index = -1;
        encounter->blind = (flags & (1u << _encounter_blind_bit)) != 0;
        encounter->deaf = (flags & (1u << _encounter_deaf_bit)) != 0;
        encounter->respawn_enabled = (flags & (1u << _encounter_respawn_enable_bit)) != 0;
        encounter->respawn_delay_ticks = 0;
        encounter->enemy_traitor = 0;
        encounter->enemy_visible = 0;
        encounter->enemy_visible_timer = -1;
        encounter->enemy_alive = 0;
        encounter->enemy_alive_timer = -1;
        encounter->corpse_ignore_time = -1;
        encounter->stand_down = 1;
        encounter->last_grenade_throw_time = -1;
        encounter->link_encounter_count = 0;
        encounter->last_active_time = -1;

        /* squads */
        int16_t squad_count = encounter_def->squads.count;
        encounter->squad_base = squad_base_counter;
        encounter->squad_count = squad_count;
        squad_base_counter += squad_count;
        for ( int16_t s = 0; s < encounter->squad_count; s = (int16_t)(s + 1) )
        {
            squad_definition *scenario_squad = (squad_definition *)encounter_def->squads.address + s;
            squad_datum *squad = &squad_array[(int16_t)(s + encounter->squad_base)];
            unsigned int squad_flags = scenario_squad->flags;
            squad->delay_timer_started = 0;
            if ( (squad_flags & (1u << _squad_delay_forever_bit)) != 0 )
                squad->delay_timer = 999;
            else
                squad->delay_timer = (int)(scenario_squad->squad_delay_timer * 30.0f);
            squad->automatic_migration_target = (squad_flags & (1u << _squad_automatic_migration_bit)) != 0;
            squad_reset_starting_locations(datum_index, s);
            if ( scenario_squad->respawn_max_actors > 0 || scenario_squad->respawn_min_actors > 0 )
            {
                int16_t respawn = scenario_squad->respawn_total_count;
                if ( !respawn )
                    respawn = 999;
                squad->respawn_actors_left = respawn;
            }
        }

        /* platoons */
        int16_t platoon_count = encounter_def->platoons.count;
        encounter->platoon_base = platoon_base_counter;
        encounter->platoon_count = platoon_count;
        platoon_base_counter += platoon_count;
        for ( int16_t p = 0; p < encounter->platoon_count; p = (int16_t)(p + 1) )
        {
            platoon_definition *scenario_platoon = (platoon_definition *)encounter_def->platoons.address + p;
            platoon_array[(int16_t)(p + encounter->platoon_base)].defending =
                (scenario_platoon->flags & (1u << _platoon_initially_defending_bit)) != 0;
        }
    }
}
