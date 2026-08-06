/* ai_scripting_vehicle_encounter @0x83772098 — assign a vehicle to an encounter+squad, so AI riding it are
 * counted in that encounter. Resolves the AI index (whole encounter / platoon / squad scope) to an
 * encounter+squad, re-homes any actor already riding the vehicle from the vehicle's previously-assigned
 * encounter, and records the new encounter+squad on the vehicle object
 * (_unit_datum.fake_encounter_index / fake_squad_index, unit_datum +0x334 / +0x336).
 *
 * DEVIATION: the decompiler renders the platoon-squad lookup result with a saturating 64-bit expression;
 * reconstructed as a plain "first squad matching the platoon, else squad 0" search.
 *
 * DEVIATION: the ai_index unpack was `(ai_index >> 8) & 0xFF` — Hex-Rays' BYTE1 expanded with the
 * little-endian value form. Disasm 0x83772130 is `extrwi r10, r4, 8, 8` (rlwinm SH=16 MB=24 ME=31), i.e.
 * (x >> 16) & 0xFF. Now spelled AI_INDEX_SUB_INDEX (headers/ai_index.h).
 *
 * DEVIATION (signature): the DB prototype says `int ai_index`; disasm 0x83772110 shifts it with `srwi`
 * (logical), so the parameter is unsigned — the corpus spelling is kept. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/scenario.h"
#include "headers/encounter_definition.h"
#include "headers/squad_definition.h"
#include "headers/actor_datum.h"
#include "headers/encounter_actor_iterator.h"
#include "headers/unit_datum.h"
#include "headers/ai_index.h"
#include "headers/blam_data_globals.h"

extern void actor_change_encounter(int actor_index, int encounter_index, int16_t squad_index);

void ai_scripting_vehicle_encounter(int unit_index, unsigned int ai_index)
{
    if ( unit_index == -1 )
        return;

    int16_t target_encounter = -1;
    int16_t target_squad = -1;
    unit_datum *vehicle_object = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    if ( ai_index != -1 && AI_INDEX_ENCOUNTER_SIGNED(ai_index) >= 0
         && AI_INDEX_ENCOUNTER_SIGNED(ai_index) < global_scenario->ai_encounters.count )
    {
        encounter_definition *encounter_def = &((encounter_definition *)global_scenario->ai_encounters.address)[(uint16_t)ai_index];
        int16_t squad = 0;
        if ( AI_INDEX_SCOPE(ai_index) == _ai_index_platoon )
        {
            /* Platoon-scoped: first squad whose squad_definition.platoon_index matches the packed
             * platoon byte, else squad 0. */
            int squad_count = encounter_def->squads.count;
            int16_t squad_search_index = 0;
            if ( squad_count > 0 )
            {
                for ( squad_search_index = 0; squad_search_index < squad_count; ++squad_search_index )
                {
                    if ( ((squad_definition *)encounter_def->squads.address)[squad_search_index].platoon_index
                         == AI_INDEX_SUB_INDEX(ai_index) )
                        break;
                }
            }
            squad = squad_search_index < squad_count ? squad_search_index : 0;
        }
        else if ( AI_INDEX_SCOPE(ai_index) == _ai_index_squad )
        {
            squad = AI_INDEX_SUB_INDEX(ai_index);
        }

        if ( squad >= 0 && squad < encounter_def->squads.count )
        {
            target_encounter = AI_INDEX_ENCOUNTER_SIGNED(ai_index);
            target_squad = squad;
        }
    }

    if ( target_encounter != -1 && target_squad != -1 )
    {
        int16_t previous_encounter = vehicle_object->unit.fake_encounter_index;
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
