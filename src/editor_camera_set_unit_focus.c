/* editor_camera_set_unit_focus @0x837C3B70 — points the editor camera at a unit. Records the focus unit
 * and caches the offset from the unit's position to the current camera position (so the camera can track
 * the unit). With unit_index == -1 the offset is zeroed. No-op if there is no editor camera. */

#include "headers/flying_camera.h"
#include "headers/real_vector3d.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/blam_data_globals.h"


void editor_camera_set_unit_focus(int unit_index)
{
    unit_focus = unit_index;
    if (!editor_camera)
        return;

    if (unit_index == -1)
    {
        unit_offset = *global_zero_vector3d;
    }
    else
    {
        object_datum *unit = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
        unit_offset.n[0] = editor_camera->position.n[0] - unit->object.bounding_sphere_center.n[0];  /* +0xA0 */
        unit_offset.n[1] = editor_camera->position.n[1] - unit->object.bounding_sphere_center.n[1];
        unit_offset.n[2] = editor_camera->position.n[2] - unit->object.bounding_sphere_center.n[2];
    }
}
