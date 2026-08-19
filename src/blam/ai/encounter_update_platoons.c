/* encounter_update_platoons @ 0x8370DB38 — per-tick platoon maintenance for one encounter. For each platoon
 * in the encounter (platoon_base..platoon_base+platoon_count) with living members: start maneuvering when its
 * maneuver rule passes; then, when not actively maneuvering (or maneuvers disabled), flip the defending state
 * to the platoon flag (bit 0x4 of the platoon def flags, inverted) once its defending rule passes.
 *
 * The scenario platoon definitions are reached through global_scenario->ai_encounters.address, indexed as
 * an encounter_definition array (176 bytes each); each encounter def's platoons tag_block holds the
 * platoon_definition array (172 bytes each), whose flags/attacking_defending_rule/maneuvering_rule are
 * named fields at +32/+48/+60. */

#include "headers/platoon_definition.h"
#include "headers/platoon_flags.h"
#include "headers/encounter_definition.h"
#include "headers/encounter_datum.h"
#include "headers/platoon_datum.h"
#include "headers/platoon_rule.h"
#include "headers/scenario.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"
#include <stdint.h>


#include "headers/platoon_rule.h"
extern uint8_t encounter_test_rule(int encounter_index, platoon_rule *rule);

void encounter_update_platoons(int encounter_index)
{
    encounter_datum *encounter = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_index);
    encounter_definition *encounter_def =
        (encounter_definition *)global_scenario->ai_encounters.address + (uint16_t)encounter_index;

    if ( encounter->platoon_count <= 0 )
        return;

    platoon_definition *platoon_definitions = (platoon_definition *)encounter_def->platoons.address;
    for ( int16_t platoon_local_index = 0; platoon_local_index < encounter->platoon_count; ++platoon_local_index )
    {
        platoon_datum *platoon = &platoon_array[(int16_t)(encounter->platoon_base + platoon_local_index)];
        if ( platoon->current_count <= 0 )
            continue;

        platoon_definition *platoon_def = &platoon_definitions[platoon_local_index];
        if ( !platoon->maneuvering )
            platoon->maneuvering = encounter_test_rule(encounter_index, &platoon_def->maneuvering_rule);

        if ( platoon->maneuver_disable || !platoon->maneuvering )
        {
            unsigned int defending = (~platoon_def->flags >> _platoon_initially_defending_bit) & 1;
            if ( platoon->defending != defending )
            {
                if ( encounter_test_rule(encounter_index, &platoon_def->attacking_defending_rule) )
                    platoon->defending = defending;
            }
        }
    }
}
