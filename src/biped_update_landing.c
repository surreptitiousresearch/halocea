/* biped_update_landing @0x837B2998 — advances the per-tick landing recovery counter for a biped,
 * fires the two landing footstep sounds at the right moment, and selects the desired landing
 * animation state. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_animation_update_data.h"
#include "headers/biped_datum.h"
#include "headers/unit_animation_state.h"
#include "headers/material_effect_event.h"
#include "headers/blam_data_globals.h"

extern uint8_t cinematic_in_progress(void);
extern void biped_make_footstep(int biped_index, int16_t event_index, int16_t contact_point_index);

void biped_update_landing(int biped_index, unit_animation_update_data *animation)
{
    /* object_header table entry is 12 bytes; datum pointer at +8 (word +2) */
    biped_datum *biped = ((biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum);

    /* landing_recovery_counter @0x4D0 (biped +0x04), landing_recovery_time @0x4D1 (biped +0x05),
     * landing @0x508 (biped +0x3C) */
    signed char landing_ticks = (signed char)((unsigned char)biped->biped.landing_recovery_counter + 1);
    signed char landing_period = (signed char)biped->biped.landing_recovery_time;
    biped->biped.landing_recovery_counter = landing_ticks;
    if (landing_ticks >= landing_period)
        biped->biped.landing = -1;

    if (!cinematic_in_progress() && !g_running_simulated_update
        && ((unsigned char)biped->biped.landing_recovery_counter == 2
            || ((unsigned __int16)biped->biped.landing == 0xFFFF
                && (signed char)biped->biped.landing_recovery_time < 2)))
    {
        biped_make_footstep(biped_index, _material_effect_biped_jump_land, 0);
        biped_make_footstep(biped_index, _material_effect_biped_jump_land, 1);
    }

    /* DEVIATION: replaces the cntlzw zero-test idiom — (cntlzw(frame - 1) & 0x20) != 0 is true
     * exactly when the landing-frame word equals 1. */
    animation->state_desired = ((unsigned __int16)biped->biped.landing == 1) ? _unit_state_land_hard : _unit_state_land_soft;
}
