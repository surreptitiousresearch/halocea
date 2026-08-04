/* encounter_control_actors @ 0x8370E258 — push the encounter's combat/maneuver state down onto every actor
 * in the encounter, then recompute dirty perception status. Walks the actor list (the encounterless list when
 * encounter_index == -1, else the encounter's first_actor_index chain via meta.next_actor_index), copies the
 * encounter's stand-down / flag bytes into each actor, clears the post-combat target fields when the
 * encounter is not in post-combat, applies the owning platoon's defending state, and — when the platoon is
 * actively maneuvering — moves the actor to its maneuver squad and issues the advance/flee stimulus.
 *
 * Actor and scenario-definition reads are by raw byte offset (verified against the decompiler at
 * 0x8370E338-0x8370E414). The scenario encounter definition is 176 bytes: squads block address at def[33]
 * (+132), platoons block address at def[36] (+144), squad count at def[32] (+128). Per-squad def is 232
 * bytes (maneuver target squad index at +78); per-platoon def is 172 bytes (flags dword at +32). */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/platoon_flags.h"
#include "headers/encounter_definition.h"
#include "headers/squad_definition.h"
#include "headers/platoon_definition.h"
#include "headers/encounter_datum.h"
#include "headers/platoon_datum.h"
#include "headers/scenario.h"
#include "headers/ai_globals.h"
#include "headers/data_array.h"
#include "headers/actor_postcombat_type.h"
#include "headers/blam_data_globals.h"


extern void actor_change_encounter(int actor_index, int encounter_index, int16_t squad_index);
extern void actor_stimulus_maneuvering(uint16_t actor_index, uint8_t advancing, uint8_t flee);
extern void encounters_update_dirty_status(void);

void encounter_control_actors(int encounter_index)
{
    encounter_datum *encounter = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_index);
    encounter_definition *encounter_def =
        (encounter_definition *)global_scenario->ai_encounters.address + (unsigned __int16)encounter_index;

    if ( !ai_globals->ai_initialized_for_map )
        return;

    int actor_index = (encounter_index == -1)
                     ? ai_globals->first_encounterless_actor_index
                     : encounter->first_actor_index;   /* recovered: *(int *)(encounter + 0x14) */

    while ( ai_globals->ai_initialized_for_map && actor_index != -1 )
    {
        int   this_actor = actor_index;
        actor_datum *actor = DATUM_GET(actor_data, actor_datum, actor_index);
        actor_index = actor->meta.next_actor_index;

        actor->external_orders.stand_down = encounter->stand_down;
        actor->external_orders.playfighting = encounter->playfighting;

        if ( !encounter->post_combat )
        {
            actor->external_orders.postcombat_type = actor_postcombat_none;
            actor->external_orders.postcombat_prop_index = -1;
        }

        __int16 platoon_local_index = actor->meta.platoon_index;
        unsigned __int8 defending = 0;
        unsigned __int8 maneuvering_active = 0;
        if ( platoon_local_index != -1 )
        {
            platoon_datum *platoon =
                &platoon_array[(__int16)(encounter->platoon_base + platoon_local_index)]; /* recovered: *(__int16 *)(encounter + 8) */
            defending = platoon->defending;
            maneuvering_active = (platoon->maneuvering && !platoon->maneuver_disable) ? 1 : 0;
        }
        actor->external_orders.defending = defending;

        if ( maneuvering_active )
        {
            platoon_definition *platoon_def = (platoon_definition *)encounter_def->platoons.address + platoon_local_index;
            __int16 squad_index = ((squad_definition *)encounter_def->squads.address
                                   + actor->meta.squad_index)->maneuver_squad_index;
            if ( squad_index >= 0 && squad_index < encounter_def->squads.count )
            {
                actor_change_encounter(this_actor, encounter_index, squad_index);
                actor_stimulus_maneuvering(this_actor,
                                           (platoon_def->flags & (1u << _platoon_advancing_maneuver_bit)) != 0,
                                           platoon_def->flags & (1u << _platoon_flee_upon_maneuver_bit));
            }
        }
    }

    encounters_update_dirty_status();
}
