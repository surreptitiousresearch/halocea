/* encounter_new @0x8370A2D8 — allocate and initialise a runtime encounter datum from a scenario
 * encounter_definition. Grabs a fresh slot from encounter_data, clears/defaults the per-encounter state
 * (team, flags-derived booleans, various -1 "none" handles), then reserves contiguous runs in the global
 * squad and platoon arrays: squad_base/platoon_base are running high-water marks bumped by this encounter's
 * squad/platoon counts. For each squad it seeds the delay timer (999 ticks if the "manual" flag is set, else
 * the definition's delay seconds * 30 tick/s), migration-target flag, respawn budget, and resets starting
 * locations. For each platoon it seeds the "defending" flag from the platoon definition.
 *
 * The encounter datum (encounter_datum, 108-byte stride), squad_datum / platoon_datum, and the
 * squad_definition (232-byte stride) / platoon_definition (172-byte stride) tag-block elements are all
 * accessed through their named struct fields. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/encounter_flags.h"
#include "headers/squad_flags.h"
#include "headers/platoon_flags.h"
#include "headers/encounter_definition.h"
#include "headers/encounter_datum.h"
#include "headers/squad_definition.h"
#include "headers/platoon_definition.h"
#include "headers/squad_datum.h"
#include "headers/platoon_datum.h"
#include "headers/blam_data_globals.h"

extern int datum_new(data_array *data);
extern void squad_reset_starting_locations(uint16_t encounter_index, int16_t squad_index);

void __fastcall encounter_new(encounter_definition *encounter_definition, int16_t *squad_base, int16_t *platoon_base)
{
    int encounter_index;
    int encounter_index_saved;
    encounter_datum *encounter;
    int squad_slot;
    __int16 squad_count;
    int squad_i;
    squad_definition *squad_def;
    squad_datum *squad;
    __int16 respawn_count;
    int platoon_count;
    int platoon_index;
    int platoon_i;
    int platoon_datum_index;

    encounter_index = datum_new(encounter_data);
    encounter_index_saved = encounter_index;
    if (encounter_index != -1)
    {
        encounter = (encounter_datum *)encounter_data->data + (unsigned __int16)encounter_index;
        squad_slot = 0; /* LOWORD(squad_slot) = 0 */
        encounter->team_index = encounter_definition->team_index;
        encounter->first_actor_index = -1;
        encounter->first_pursuit_index = -1;
        encounter->blind = (encounter_definition->flags & (1u << _encounter_blind_bit)) != 0;
        encounter->deaf = (encounter_definition->flags & (1u << _encounter_deaf_bit)) != 0;
        encounter->respawn_enabled = (encounter_definition->flags & (1u << _encounter_respawn_enable_bit)) != 0;
        encounter->stand_down = 1;
        encounter->enemy_visible_timer = -1;
        encounter->enemy_alive_timer = -1;
        encounter->corpse_ignore_time = -1;
        encounter->last_grenade_throw_time = -1;
        encounter->last_active_time = -1;
        encounter->respawn_delay_ticks = 0;
        encounter->enemy_traitor = 0;
        encounter->enemy_visible = 0;
        encounter->enemy_alive = 0;
        encounter->link_encounter_count = 0;
        squad_count = encounter_definition->squads.count;
        encounter->squad_count = squad_count;
        encounter->squad_base = *squad_base;
        *squad_base += squad_count;
        if (encounter->squad_count > 0)
        {
            squad_i = 0;
            do
            {
                squad_def = &((squad_definition *)encounter_definition->squads.address)[squad_i];
                squad = &squad_array[(__int16)(encounter->squad_base + squad_slot)];
                squad->delay_timer_started = 0;
                if ((squad_def->flags & (1u << _squad_delay_forever_bit)) != 0)
                    squad->delay_timer = 999;
                else
                    squad->delay_timer = (int)(squad_def->squad_delay_timer * 30.0f);
                squad->automatic_migration_target = (squad_def->flags & (1u << _squad_automatic_migration_bit)) != 0;
                squad_reset_starting_locations(encounter_index_saved, squad_slot);
                if (squad_def->respawn_max_actors > 0 || squad_def->respawn_min_actors > 0)
                {
                    respawn_count = squad_def->respawn_total_count;
                    if (!respawn_count)
                        respawn_count = 999;
                    squad->respawn_actors_left = respawn_count;
                }
                squad_slot = (__int16)(squad_i + 1);
                squad_i = squad_slot;
            }
            while (squad_slot < encounter->squad_count);
        }
        platoon_count = encounter_definition->platoons.count;
        platoon_index = 0; /* LOWORD(platoon_index) = 0 */
        encounter->platoon_count = platoon_count;
        encounter->platoon_base = *platoon_base;
        *platoon_base += platoon_count;
        if (encounter->platoon_count > 0)
        {
            platoon_i = 0;
            do
            {
                platoon_datum_index = (__int16)(platoon_index + encounter->platoon_base);
                platoon_index = (__int16)(platoon_i + 1);
                platoon_array[platoon_datum_index].defending =
                    (((platoon_definition *)encounter_definition->platoons.address)[platoon_i].flags
                        & (1u << _platoon_initially_defending_bit)) != 0;
                platoon_i = platoon_index;
            }
            while (platoon_index < encounter->platoon_count);
        }
    }
}
