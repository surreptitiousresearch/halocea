/* object_restore_body @0x836B1420 — restore an object to full body health. If the object is not
 * already dead (damage_flags bit 2 clear) and its body vitality is below full, the vitality is set to
 * 1.0 and the "body changed" update bit (flags 0x4000000) is raised. Returns 1 if a restore happened,
 * 0 otherwise. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_damage_flags.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"


uint8_t object_restore_body(int object_index)
{
    object_datum *object =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    if ((object->object.damage_flags & (1u << _object_dead_bit)) == 0 && object->object.body_vitality < 1.0f)
    {
        object->object.body_vitality = 1.0f;
        object->object.flags |= (1u << _object_force_baseline_update_bit);
        return 1;
    }
    return 0;
}
