/* first_person_weapon_center_flashlight @0x8369F050 — flashlight anchor for the first-person view: from
 * the visible first-person weapon's "flashlight" marker, output the marker's forward and up axes and a
 * position pulled back half a world unit along the forward axis (position - 0.5*forward). Outputs are left
 * untouched when there is no visible first-person weapon or no flashlight marker. */

#include <stdint.h>
#include "headers/first_person_weapon.h"
#include "headers/object_marker.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"


extern int16_t first_person_weapon_index_from_unit_index(int unit_index);
extern int16_t first_person_weapon_get_marker_by_name(int weapon_index, const char *name, object_marker *markers, int16_t maximum_marker_count);

void first_person_weapon_center_flashlight(int unit_index, real_point3d *position, real_vector3d *forward,
        real_vector3d *up)
{
    int16_t first_person_weapon_index = first_person_weapon_index_from_unit_index(unit_index);
    if ( first_person_weapon_index == -1 )
        return;

    first_person_weapon *fp_weapon = &first_person_weapons[first_person_weapon_index];
    if ( !fp_weapon->visible )
        return;

    object_marker marker;
    if ( first_person_weapon_get_marker_by_name(fp_weapon->weapon_index, "flashlight", &marker, 1) > 0 )
    {
        position->n[0] = -(marker.matrix.n[0][0] * 0.5f - marker.matrix.n[3][0]);
        position->n[1] = -(marker.matrix.n[0][1] * 0.5f - marker.matrix.n[3][1]);
        position->n[2] = -(marker.matrix.n[0][2] * 0.5f - marker.matrix.n[3][2]);
        forward->n[0] = marker.matrix.n[0][0];
        forward->n[1] = marker.matrix.n[0][1];
        forward->n[2] = marker.matrix.n[0][2];
        up->n[0] = marker.matrix.n[2][0];
        up->n[1] = marker.matrix.n[2][1];
        up->n[2] = marker.matrix.n[2][2];
    }
}
