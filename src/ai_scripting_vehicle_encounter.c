/* ai_scripting_vehicle_encounter @0x83772098 — assign a vehicle to an encounter+squad, so AI riding it are
 * counted in that encounter. Resolves the AI index (whole encounter / platoon / squad scope) to an
 * encounter+squad, re-homes any actor already riding the vehicle from the vehicle's previously-assigned
 * encounter, and records the new encounter+squad on the vehicle object (+0x334 / +0x336).
 *
 * DEVIATION: the decompiler renders the platoon-squad lookup result with a saturating 64-bit expression;
 * reconstructed as a plain "first squad matching the platoon, else squad 0" search. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/scenario.h"
#include "headers/encounter_definition.h"
#include "headers/squad_definition.h"
#include "headers/actor_datum.h"
#include "headers/encounter_actor_iterator.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"

extern void actor_change_encounter(int actor_index, int encounter_index, int16_t squad_index);

void ai_scripting_vehicle_encounter(int unit_index, unsigned int ai_index)
{
    if ( unit_index == -1 )
        return;

    __int16 target_encounter = -1;
    __int16 target_squad = -1;
    unit_datum *vehicle_object = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    if ( ai_index != -1 && (__int16)ai_index >= 0 && (__int16)ai_index < global_scenario->ai_encounters.count )
    {
        encounter_definition *encounter_def = &((encounter_definition *)global_scenario->ai_encounters.address)[(unsigned __int16)ai_index];
        __int16 squad = 0;
        if ( ai_index >> 30 == 1 )
        {
            /* Platoon-scoped: first squad whose platoon (squad def +0x22) matches byte 1, else squad 0. */
            int squad_count = encounter_def->squads.count;
            int found = 0;
            if ( squad_count > 0 )
            {
                for ( found = 0; found < squad_count; found = (__int16)(found + 1) )
                {
                    if ( ((squad_definition *)encounter_def->squads.address)[found].platoon_index == ((ai_index >> 8) & 0xFF) )
                        break;
                }
            }
            squad = (__int16)found < squad_count ? (__int16)found : 0;
        }
        else if ( ai_index >> 30 == 2 )
        {
            squad = (ai_index >> 8) & 0xFF;
        }

        if ( squad >= 0 && squad < encounter_def->squads.count )
        {
            target_encounter = ai_index;
            target_squad = squad;
        }
    }

    if ( target_encounter != -1 && target_squad != -1 )
    {
        __int16 previous_encounter = vehicle_object->unit.fake_encounter_index;
        if ( previous_encounter != -1 )
        {
            encounter_actor_iterator iterator;
            encounter_actor_iterator_new(&iterator, previous_encounter);
            for ( actor_datum *actor = encounter_actor_iterator_next(&iterator);
                  actor;
                  actor = encounter_actor_iterator_next(&iterator) )
            {
                if ( actor->input.vehicle_index == unit_index )
                    actor_change_encounter(iterator.index, target_encounter, target_squad);
            }
        }
    }

    vehicle_object->unit.fake_encounter_index = target_encounter;
    vehicle_object->unit.fake_squad_index = target_squad;
}
