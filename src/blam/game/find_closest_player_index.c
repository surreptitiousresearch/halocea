/* find_closest_player_index @0x83746738 — resolve the player index a local player's HUD/camera should be
 * "closest" to (for e.g. spectator/kill-cam framing): the local player's current autoaim target if one is
 * locked, else the nearest valid target found by a facing-direction object sweep, filtered through the
 * same autoaim gate (angle/distance thresholds) used for weapon targeting.
 *
 * DEVIATION: the DB's real prototype takes only `player_index`; the decompiler's a2..a12 are a stack
 * over-read artifact (this function has no other real arguments).
 * The candidates are unit-type objects, so the datum is typed as unit_datum: floats[23..25] (byte offset
 * 92..100) are object.position, and floats[223] (byte offset 892) is unit.active_camouflage — past the
 * 500-byte object base, in the unit sub-struct of the same pool slot.
 * NOTE: `*((_DWORD*)player+31)` (byte offset 124) is player_datum.multiplayer.player_display_index. */

#include <stdint.h>
#include "headers/player_datum.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/ppc_intrinsics.h"
#include "headers/blam_data_globals.h"


extern float player_control_get_autoaim_level(int16_t local_player_index);
extern int player_control_get_target_object_index(int16_t local_player_index);
extern int player_index_from_unit_index(int unit_index);
extern void unit_get_camera_position(int unit_index, real_point3d *camera_position);
extern real_vector3d *player_control_get_facing_direction(int16_t local_player_index, real_vector3d *direction);
extern int find_objects_from_point_vector(const real_point3d *position, const real_vector3d *direction, uint8_t (*add_object_function)(int, void *), void *custom_data, int maximum_object_count, int *object_indices);
extern uint8_t find_closest_player_callback(int object_index, int *custom_data);
extern uint8_t autoaim_compute_target(int object_index, const real_point3d *position, const real_vector3d *direction, int ignore_object_index, real_point3d *target_position, real_vector3d *target_direction, float *target_distance, float *target_angle);

int find_closest_player_index(int player_index)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    int target_object_index = -1;
    if (player->local_player_index != -1 && player_control_get_autoaim_level(player->local_player_index) > 0.0f)
    {
        target_object_index = player_control_get_target_object_index(player->local_player_index);
        if (target_object_index != -1)
            return player_index_from_unit_index(target_object_index);
    }

    real_point3d  camera_position;
    real_vector3d facing_direction;
    unit_get_camera_position(player->unit_index, &camera_position);
    player_control_get_facing_direction(player->local_player_index, &facing_direction);

    int object_indices[44];
    int object_count = find_objects_from_point_vector(&camera_position, &facing_direction,
            find_closest_player_callback, &player_index, 32, object_indices);
    if (object_count <= 0)
        return target_object_index;

    /* recovered: *(const int *)((const char *)player + 124) -> player->multiplayer.player_display_index */
    int requesting_player_field = player->multiplayer.player_display_index;

    for (int i = 0; i < object_count; ++i)
    {
        int object_index = object_indices[i];
        /* candidates are unit-type objects; unit_datum covers both the object base and unit.active_camouflage */
        unit_datum *object = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

        float dx = object->object.position.n[0] - camera_position.n[0];
        float dy = object->object.position.n[1] - camera_position.n[1];
        float dz = object->object.position.n[2] - camera_position.n[2];
        float distance_squared = dx * dx + dy * dy + dz * dz;

        real_point3d  target_position;
        real_vector3d target_direction;
        float         target_distance;
        float         target_angle;

        if ((object->unit.active_camouflage < 1.0f || requesting_player_field == player_index_from_unit_index(object_index))
            && autoaim_compute_target(object_index, &camera_position, &facing_direction, player->unit_index,
                                      &target_position, &target_direction, &target_distance, &target_angle)
            && __fabs(target_angle) < 0.13083334f
            && distance_squared < 400.0f
            && distance_squared < 900.0f)
        {
            target_object_index = object_index;
        }
    }

    if (target_object_index == -1)
        return target_object_index;
    return player_index_from_unit_index(target_object_index);
}
