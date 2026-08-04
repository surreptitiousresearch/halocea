/* ai_scripting_count_internal @0x8376FC20 — count the living actors addressed by an AI index, with the
 * scope (encounter / platoon / squad) selected by the top two bits of the index and the platoon/squad index
 * carried in byte 1. count_type selects which population (enum ai_count_type): living = total current count, swarm, nonswarm =
 * non-swarm count (current minus swarm, clamped at 0). Optionally also reports the original (spawned) count
 * and current strength fraction. Returns the requested count (0 if the index is out of range). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/scenario.h"
#include "headers/squad_datum.h"
#include "headers/platoon_datum.h"
#include "headers/encounter_datum.h"
#include "headers/ai_count_type.h"
#include "headers/blam_data_globals.h"


int ai_scripting_count_internal(unsigned int ai_index, int16_t count_type, int *original_count_reference, float *strength_reference)
{
    int result = 0;
    int original_count = 0;
    float current_strength_fraction = 0.0f;

    if ( ai_index == -1 )
        goto done;

    if ( !(ai_index >> 30) )
    {
        /* Encounter scope. */
        if ( (unsigned __int16)ai_index >= global_scenario->ai_encounters.count )
            goto done;
        encounter_datum *encounter = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, ai_index);
        if ( count_type == _ai_count_living )
            result = encounter->current_count;
        else if ( count_type == _ai_count_swarm )
            result = encounter->current_swarm_count;
        else
        {
            int diff = encounter->current_count - encounter->current_swarm_count;
            result = diff < 0 ? 0 : diff;                        /* non-swarm count, clamped */
        }
        original_count = encounter->original_count;
        current_strength_fraction = encounter->current_strength_fraction;
    }
    else if ( ai_index >> 30 == 1 )
    {
        /* Platoon scope. */
        if ( (unsigned __int16)ai_index < global_scenario->ai_encounters.count )
        {
            encounter_datum *encounter = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, ai_index);
            if ( (int)((ai_index >> 8) & 0xFF) < encounter->platoon_count )
            {
                platoon_datum *platoon = &platoon_array[(__int16)(encounter->platoon_base + ((ai_index >> 8) & 0xFF))];
                current_strength_fraction = platoon->current_strength_fraction;
                original_count = platoon->original_count;
                if ( count_type == _ai_count_living )
                    result = platoon->current_count;
                else if ( count_type == _ai_count_swarm )
                    result = platoon->current_swarm_count;
                else
                {
                    int diff = platoon->current_count - platoon->current_swarm_count;
                    result = diff < 0 ? 0 : diff;
                }
                goto report;
            }
        }
    }
    else
    {
        /* Squad scope. */
        if ( (unsigned __int16)ai_index < global_scenario->ai_encounters.count )
        {
            encounter_datum *encounter = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, ai_index);
            if ( (int)((ai_index >> 8) & 0xFF) < encounter->squad_count )
            {
                squad_datum *squad = &squad_array[(__int16)(encounter->squad_base + ((ai_index >> 8) & 0xFF))];
                current_strength_fraction = squad->current_strength_fraction;
                original_count = squad->original_count;
                if ( count_type == _ai_count_living )
                    result = squad->current_count;
                else if ( count_type == _ai_count_swarm )
                    result = squad->current_swarm_count;
                else
                {
                    int diff = squad->current_count - squad->current_swarm_count;
                    result = diff < 0 ? 0 : diff;
                }
                goto report;
            }
        }
    }

report:
done:
    if ( original_count_reference )
        *original_count_reference = original_count;
    if ( strength_reference )
        *strength_reference = current_strength_fraction;
    return result;
}
