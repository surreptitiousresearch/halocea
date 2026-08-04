/* actor_activation_debug_color @0x8371D680 — debug color for an actor's activation state: fully active
 * (byte +8) actors show blue (asleep/frozen, byte +19), yellow/green (byte +18, dormant — green if a
 * countdown word +20 is positive, else yellow), or white (fully awake). Inactive actors (no encounter,
 * dword +52 == -1, or the encounter's structure BSP index (ai_encounters +126, word) doesn't match the
 * current one) show red; those still relevant to the current BSP show purple. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/scenario.h"
#include "headers/encounter_definition.h"
#include "headers/real_argb_color.h"
#include "headers/blam_data_globals.h"



const real_argb_color * actor_activation_debug_color(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( actor->meta.active )
    {
        if ( actor->meta.dormant )
            return global_real_argb_blue;
        if ( actor->meta.dormant_desire )
            return actor->meta.become_dormant_timer <= 0 ? global_real_argb_yellow : global_real_argb_green;
        return global_real_argb_white;
    }

    int encounter_index = actor->meta.encounter_index;
    unsigned __int8 relevant_to_current_bsp = 0;

    if ( encounter_index != -1 )
    {
        __int16 encounter_bsp_index = ((encounter_definition *)global_scenario->ai_encounters.address)
                                     [(unsigned __int16)encounter_index].runtime_structure_bsp_reference_index;
        relevant_to_current_bsp = encounter_bsp_index != -1 && encounter_bsp_index != global_structure_bsp_index;
    }

    return relevant_to_current_bsp ? global_real_argb_red : global_real_argb_purple;
}
