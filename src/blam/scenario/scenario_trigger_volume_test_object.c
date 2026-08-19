/* scenario_trigger_volume_test_object @0x83703E98 — true if the object's origin lies inside the given
 * trigger volume. Returns false for a null object. The origin is the real_point3d at object data +160.
 *
 * GAME-VERSION NOTE (as-built, not a defect — do not "fix"): this build tests the object's BOUNDING
 * SPHERE CENTER, not its position. Ours emits `addi r4, r11, 0xA0` at 0x83703EC8, immediately before
 * the tail `b scenario_trigger_volume_test_point`; 0xA0 == object_datum.object.bounding_sphere_center
 * (0x04 + 0x9C, types_members). A NEWER PC build ("Tag Test", same function @0x00872980) adds 0x5C
 * instead, which in this identical layout is object_datum.object.position (0x04 + 0x58) — a later
 * semantic change, not a correction. Bipeds whose collision center sits above their origin therefore
 * gate differently between the two builds; that behavioural delta is expected here. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"

#include "headers/real_point3d.h"
extern int scenario_trigger_volume_test_point(int16_t trigger_volume_index, const real_point3d *position);

/* attested uint8_t: tail-branch to scenario_trigger_volume_test_point; 8/8 callers clrlwi r3,24 */
uint8_t scenario_trigger_volume_test_object(int16_t trigger_volume_index, int object_index)
{
    if ( object_index == -1 )
        return 0;
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    /* +160 = object_datum.object.bounding_sphere_center (0x04 + 0x9C) */
    return scenario_trigger_volume_test_point(trigger_volume_index, &object->object.bounding_sphere_center);
}
