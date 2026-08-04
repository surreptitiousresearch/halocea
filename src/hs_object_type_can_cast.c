/* hs_object_type_can_cast @0x8368D6D8 — test whether a script object reference of `actual_type` can
 * be used where `desired_type` is expected: true when the actual type's object-type mask is a subset
 * of the desired type's mask (containment test via hs_object_type_masks[]). */

#include <stdint.h>
#include "headers/hs_object_type_masks.h"

int hs_object_type_can_cast(int16_t actual_type, int16_t desired_type)
{
    return hs_object_type_masks[actual_type]
        == (hs_object_type_masks[desired_type] & hs_object_type_masks[actual_type]);
}
