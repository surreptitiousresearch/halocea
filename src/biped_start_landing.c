/* biped_start_landing @0x837AECA4 — evaluate a fall's landing_velocity (in game units/tick, scaled from
 * world units/sec by 1/30) against the biped definition's soft/hard landing thresholds and, if it's at
 * least a soft landing, set up the landing recovery: a hard landing (velocity 1) scales its recovery time
 * across [minimum_hard_landing_velocity, maximum_hard_landing_velocity] using maximum_hard_landing_time; a
 * soft landing (velocity 0) scales across [minimum_soft_landing_velocity, minimum_hard_landing_velocity]
 * using maximum_soft_landing_time. No-op (and no landing state change) below the soft-landing threshold, or
 * when the velocity range for the selected landing type is zero/negative.
 *
 * DEVIATION: the decompiler rendered the final float-to-byte store as `(unsigned int)(int)(...) >> 24`.
 * Disasm shows the standard fctiwz-then-stack-reload float-to-int idiom (convert, store the 8-byte result,
 * reload the low 32-bit half) followed by a plain `stb` of that integer's low byte — no shift is actually
 * applied; the decompiler misread the stack-reload sequence. Restored as a plain truncating cast. */

#include "headers/biped_datum.h"
#include "headers/biped_definition.h"
#include "headers/game_time_constants.h"

void biped_start_landing(biped_datum *biped, const biped_definition *biped_definition, float landing_velocity)
{
    const float ticks_per_second_scale = SECONDS_PER_TICK; /* 1/30 */

    if ( landing_velocity < biped_definition->biped.minimum_soft_landing_velocity * ticks_per_second_scale )
        return;

    __int16 landing_type;
    float velocity_range;
    float recovery_time_scale;

    if ( landing_velocity >= biped_definition->biped.minimum_hard_landing_velocity * ticks_per_second_scale )
    {
        landing_type = 1;
        velocity_range = biped_definition->biped.maximum_hard_landing_velocity * ticks_per_second_scale
            - biped_definition->biped.minimum_hard_landing_velocity * ticks_per_second_scale;
        recovery_time_scale = biped_definition->biped.maximum_hard_landing_time;
    }
    else
    {
        landing_velocity -= biped_definition->biped.minimum_soft_landing_velocity * ticks_per_second_scale;
        recovery_time_scale = biped_definition->biped.maximum_soft_landing_time;
        velocity_range = biped_definition->biped.minimum_hard_landing_velocity * ticks_per_second_scale
            - biped_definition->biped.minimum_soft_landing_velocity * ticks_per_second_scale;
        landing_type = 0;
    }

    if ( velocity_range <= 0.0f )
        return;

    float recovery_ticks = recovery_time_scale * 30.0f;
    float fraction = landing_velocity / velocity_range;
    if ( fraction < 0.0f )
        fraction = 0.0f;
    else if ( fraction > 1.0f )
        fraction = 1.0f;

    biped->biped.landing = landing_type;
    biped->biped.landing_recovery_counter = 0;
    /* (int) models the fctiwz float->int; (char) the stb low-byte store. Redundant middle
     * (unsigned int) of the decompiler chain removed. */
    biped->biped.landing_recovery_time = (char)(int)(recovery_ticks * fraction);
}
