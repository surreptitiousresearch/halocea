/* weapon_effect_looping_new @0x836D9DB0 — create a looping effect attached to a weapon object. When the
 * weapon is invisible (attached to a carrier) the effect is attached to the parent object instead. Returns
 * the looping-effect index, or -1 if there is no effect or no valid host object. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"

extern int effect_new_looping(int definition_index, int object_index, int16_t scale_a_function_index, int16_t scale_b_function_index, int16_t change_color_index);
extern int weapon_get_owner_object_index(int weapon_index);

int weapon_effect_looping_new(int weapon_index, int effect_index)
{
    if ( effect_index == -1 )
        return -1;

    int host_object_index = weapon_index;
    weapon_datum *weapon = (weapon_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;

    if ( (weapon->object.flags & (1u << _object_invisible_bit)) != 0 )
    {
        int parent_object_index = weapon->object.parent_object_index;
        if ( parent_object_index != -1 )
            host_object_index = parent_object_index;
    }

    weapon_get_owner_object_index(weapon_index);   /* decompiler: result discarded */

    if ( host_object_index == -1 )
        return -1;
    return effect_new_looping(effect_index, host_object_index, -1, -1, -1);
}
