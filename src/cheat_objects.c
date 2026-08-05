/* cheat_objects @0x83699F90 — spawn one of each listed object tag in an arc in front of the acting player.
 * Objects are fanned out around the player's facing direction at a 1.5 world-unit radius, 0.8 units up,
 * with an angular spacing of (2*pi / count) capped at pi/8, and all share the player's orientation. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/game_globals_tag_reference.h"
#include "headers/object_placement_data.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/math_constants.h"
#include "headers/blam_data_globals.h"

extern int cheat_player_index(void);
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern void object_get_orientation(int object_index, real_vector3d *forward, real_vector3d *up);
extern void object_placement_data_new(object_placement_data *data, int definition_index, int owner_object_index);
extern int object_new(object_placement_data *data);

extern double atan2(double y, double x);
extern double cos(double x);
extern double sin(double x);

void cheat_objects(const game_globals_tag_reference *references, int16_t name_count)
{
    int player_index = cheat_player_index();
    if ( player_index == -1 )
        return;

    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    int unit_index = player->unit_index;

    real_point3d origin;
    real_vector3d forward, up;
    object_get_origin(unit_index, &origin);
    object_get_orientation(unit_index, &forward, &up);

    if ( name_count <= 0 )
        return;

    float angle_step = TWO_PI / (float)name_count;
    if ( angle_step > 0.39269909f )
        angle_step = 0.39269909f;
    float base_angle = (float)atan2(forward.n[1], forward.n[0]);

    for ( int i = 0; i < name_count; i = (int16_t)(i + 1) )
    {
        int tag_index = references[i].reference.index;
        if ( tag_index == -1 )
            continue;

        float angle = ((float)(i - name_count / 2) * angle_step) + base_angle;

        object_placement_data placement;
        object_placement_data_new(&placement, tag_index, -1);
        placement.position.n[0] = ((float)cos(angle) * 1.5f) + origin.n[0];
        placement.position.n[1] = ((float)sin(angle) * 1.5f) + origin.n[1];
        placement.position.n[2] = origin.n[2] + 0.80000001f;
        placement.forward = forward;
        placement.up = up;
        object_new(&placement);
    }
}
