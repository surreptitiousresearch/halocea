/* biped_update_baseline @0x837ADC40 — snapshot a biped object's network-baseline copy: record body/shield
 * vitality (shield scaled by 1/3), grenade counts and the shield-stun-positive flag, advance the baseline
 * generation index, mark the baseline valid, and reset the message index.
 *
 * DEVIATION: the flag is `neg r5,r8` / `andc r11,r5,r8` / `srwi r8,r11,31` @0x837ADC9C, i.e. bit 31 of
 * `(-v & ~v)`, which for the sign-extended 16-bit `v` is set iff `v > 0`; written as `shield_stun_ticks > 0`
 * (was `< 0`, which inverted the wire bit and disagreed with biped_build_update_delta.c). */

#include <stdint.h>
#include "headers/biped_datum.h"
#include "headers/object_type.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

void biped_update_baseline(int object_index)
{
    biped_datum *object = object_try_and_get_and_verify_type(object_index, object_mask_biped);

    if ( object )
    {
        int16_t shield_stun_ticks = object->object.shield_stun_ticks;
        float shield_vitality = object->object.shield_vitality;
        int16_t grenade_counts = object->unit.grenade_counts[0];
        uint8_t next_generation = object->biped.baseline_index + 1;

        object->biped.baseline.body_vitality = object->object.body_vitality;
        object->biped.baseline.shield_vitality = shield_vitality * 0.33333334f;
        object->biped.baseline_valid = 1;
        object->biped.baseline_index = next_generation;
        object->biped.message_index = 0;
        object->biped.baseline.shield_stun_ticks_greater_than_zero = shield_stun_ticks > 0;
        object->biped.baseline.grenade_counts[0] = grenade_counts;
    }
}
