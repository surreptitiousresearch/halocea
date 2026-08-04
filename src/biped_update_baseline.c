/* biped_update_baseline @0x837ADC50 — snapshot a biped object's network-baseline copy: record body/shield
 * vitality (shield scaled by 1/3), grenade counts and the shield-stun sign flag, advance the baseline
 * generation index, mark the baseline valid, and reset the message index.
 *
 * DEVIATION: the shield-stun flag is `shield_stun_ticks < 0` (`(-v2 & ~v2) < 0` is true iff the sign bit
 * is set), reproduced per the decompiler's arithmetic. */

#include "headers/biped_datum.h"
#include "headers/object_type.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

void biped_update_baseline(int object_index)
{
    biped_datum *object = object_try_and_get_and_verify_type(object_index, object_mask_biped);

    if ( object )
    {
        __int16 shield_stun_ticks = object->object.shield_stun_ticks;
        float shield_vitality = object->object.shield_vitality;
        __int16 grenade_counts = object->unit.grenade_counts[0];
        unsigned __int8 next_generation = object->biped.baseline_index + 1;

        object->biped.baseline.body_vitality = object->object.body_vitality;
        object->biped.baseline.shield_vitality = shield_vitality * 0.33333334f;
        object->biped.baseline_valid = 1;
        object->biped.baseline_index = next_generation;
        object->biped.message_index = 0;
        object->biped.baseline.shield_stun_ticks_greater_than_zero = shield_stun_ticks < 0;
        object->biped.baseline.grenade_counts[0] = grenade_counts;
    }
}
