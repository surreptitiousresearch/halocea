/* object_double_charge_shield @0x836B14A4 — grant an overshield to an object. Only acts when the
 * current shield vitality is at or below full (<= 1.0). It raises the overshield flag (damage_flags
 * 0x10), nudges a fully-depleted shield off zero so it can recharge, clears the shield recharge delay
 * (shield_stun_ticks), and marks the shield as changed (force_shield_update). Returns whether the
 * overshield was applicable. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_damage_flags.h"
#include "headers/blam_data_globals.h"


uint8_t object_double_charge_shield(int object_index)
{
    object_datum *object =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    float shield = object->object.shield_vitality;
    int applicable = shield <= 1.0f;
    if (applicable)
    {
        object->object.damage_flags |= (1u << _object_shield_over_charging_bit);
        if (shield == 0.0f)
            object->object.shield_vitality = 0.0099999998f;
        object->object.shield_stun_ticks = 0;
        object->object.force_shield_update = 1;
    }
    return applicable;
}
