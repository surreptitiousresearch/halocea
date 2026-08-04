/* first_person_weapon_adjust_light @0x8369F128 — resolve a named marker on the local player's own visible
 * first-person weapon into world position/forward/up (for positioning a muzzle-flash-style dynamic light).
 * Fails (0) unless: the weapon's parent unit has a controlling player, that player is the currently
 * rendering local player, their first-person weapon is visible, and the named marker actually resolves. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/render_globals.h"
#include "headers/first_person_weapon.h"
#include "headers/object_marker.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/unit_datum.h"
#include "headers/blam_data_globals.h"


extern int16_t first_person_weapon_get_marker_by_name(int weapon_index, const char *name, object_marker *markers, int16_t maximum_marker_count);

uint8_t first_person_weapon_adjust_light(int weapon_index, const char *marker_name, real_point3d *position, real_vector3d *forward, real_vector3d *up)
{
    object_datum *weapon_object =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    int parent_object_index = weapon_object->object.parent_object_index;
    unit_datum *parent_object =
        (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, parent_object_index)->datum;
    int controlling_player_index = parent_object->unit.player_index;

    if ( controlling_player_index == -1 )
        return 0;

    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, controlling_player_index);
    __int16 local_player_index = player->local_player_index;

    object_marker marker;
    if ( local_player_index == -1 || local_player_index != render.local_player_index
      || !first_person_weapons[local_player_index].visible
      || first_person_weapon_get_marker_by_name(weapon_index, marker_name, &marker, 1) <= 0 )
    {
        return 0;
    }

    position->n[0] = marker.matrix.n[3][0];
    position->n[1] = marker.matrix.n[3][1];
    position->n[2] = marker.matrix.n[3][2];
    forward->n[0] = marker.matrix.n[0][0];
    forward->n[1] = marker.matrix.n[0][1];
    forward->n[2] = marker.matrix.n[0][2];
    up->n[0] = marker.matrix.n[2][0];
    up->n[1] = marker.matrix.n[2][1];
    up->n[2] = marker.matrix.n[2][2];
    return 1;
}
