/* player_aim_projectile @0x836C6460 — compute the world-space firing direction for a player's projectile,
 * applying aim-assist magnetism and weapon deviation. It fetches the player's aiming unit, its zoom level and
 * aim-assist parameters; if aim assist is enabled it queries aim_assist against the deterministic camera and, on
 * a hit, points the aim direction at the target (falling back to the raw input direction if that degenerates) and
 * records the target's autoaim magnetism and object index. It then casts a collision ray from a point ahead of
 * the camera (camera position advanced by the camera-to-unit distance along the normalized forward) out to
 * 128 * forward, forms a direction toward the impact point, blends it with the magnetism-adjusted aim direction,
 * and finally pins the result into the weapon's deviation cone (writing the adjusted direction back through the
 * `direction` out-parameter). Returns the hit object index (-1 if none) and stamps the player's last-aim fields.
 *
 * DEVIATION 1: the binary reuses one stack buffer for both the aim_assist_target output and the later
 * collision_result output; modeled as two separate locals (aim_target / collision) since their lifetimes do not
 * overlap. DEVIATION 2: fast_normals_interpolate's result pointer and pin_normal_to_cone3d's result pointer were
 * mis-shown by the decompiler as phantom r5 locals (the float args burn the r5/r6 GPR shadow); disasm
 * (0x836C66E8/0x836C6714) shows the real results are &camera_forward_copy and `direction` respectively. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/collision_test_flags.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/player_datum.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/aim_assist_parameters.h"
#include "headers/aim_assist_target.h"
#include "headers/collision_result.h"
#include <math.h>
#include "headers/blam_data_globals.h"
extern float __fsqrts(float);


extern int unit_get_aiming_unit_index(int unit_index);
extern int unit_get_zoom_level(int unit_index);
extern uint8_t unit_get_aim_assist_parameters(int unit_index, int16_t zoom_level, aim_assist_parameters *parameters);
extern int director_camera_deterministic(int unit_index, real_point3d *position, real_vector3d *forward);
extern uint8_t aim_assist(const aim_assist_parameters *parameters, const real_point3d *position, const real_vector3d *direction, int ignore_object_index, int16_t ignore_team_index, aim_assist_target *target);
extern float normalize3d(real_vector3d *v);
extern uint8_t collision_test_vector(unsigned int flags, const real_point3d *point, const real_vector3d *vector, int ignore_object_index, collision_result *collision);
extern void fast_normals_interpolate(const real_vector3d *a, const real_vector3d *b, float t,
        real_vector3d *result);
extern unsigned __int8 pin_normal_to_cone3d(const real_vector3d *normal, const real_vector3d *direction,
        float sine, float cosine, real_vector3d *result);
extern int game_time_get(void);

int player_aim_projectile(int player_index, const real_point3d *position, real_vector3d *direction)
{
    int hit_object_index = -1;
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    int aiming_unit_index = unit_get_aiming_unit_index(player->unit_index);
    __int16 zoom_level = unit_get_zoom_level(aiming_unit_index);

    aim_assist_parameters aim_params;
    if ( !unit_get_aim_assist_parameters(aiming_unit_index, zoom_level, &aim_params) )
    {
        player->aim_assist_unit_index = hit_object_index;
        player->aim_assist_timestamp = game_time_get();
        return hit_object_index;
    }

    real_point3d camera_position;
    real_vector3d camera_forward;
    director_camera_deterministic(player->unit_index, &camera_position, &camera_forward);

    real_vector3d aim_direction;
    aim_direction.n[0] = direction->n[0];
    aim_direction.n[1] = direction->n[1];
    aim_direction.n[2] = direction->n[2];

    float magnetism = 0.0f;
    aim_assist_target aim_target;
    if ( aim_assist(&aim_params, &camera_position, &camera_forward, player->unit_index, player->team_index, &aim_target) )
    {
        aim_direction.n[0] = aim_target.position.n[0] - position->n[0];
        aim_direction.n[1] = aim_target.position.n[1] - position->n[1];
        aim_direction.n[2] = aim_target.position.n[2] - position->n[2];
        if ( normalize3d(&aim_direction) == 0.0f )
        {
            aim_direction.n[0] = direction->n[0];
            aim_direction.n[1] = direction->n[1];
            aim_direction.n[2] = direction->n[2];
        }
        magnetism = aim_target.autoaim_level;
        hit_object_index = aim_target.object_index;
    }

    float camera_x = camera_position.n[0];
    float camera_y = camera_position.n[1];
    float camera_z = camera_position.n[2];

    real_vector3d camera_forward_copy = camera_forward;
    object_datum *aiming_unit = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, aiming_unit_index)->datum;
    float camera_to_unit_distance = __fsqrts(
            (((camera_position.n[0] - aiming_unit->object.position.n[0]) * (camera_position.n[0] - aiming_unit->object.position.n[0]))
                    + (((camera_position.n[2] - aiming_unit->object.position.n[2]) * (camera_position.n[2] - aiming_unit->object.position.n[2]))
                            + ((camera_position.n[1] - aiming_unit->object.position.n[1]) * (camera_position.n[1] - aiming_unit->object.position.n[1])))));
    normalize3d(&camera_forward_copy);

    real_vector3d camera_forward_scaled;
    camera_forward_scaled.n[0] = camera_forward.n[0] * 128.0f;
    camera_forward_scaled.n[1] = camera_forward.n[1] * 128.0f;
    camera_forward_scaled.n[2] = camera_forward.n[2] * 128.0f;

    camera_position.n[0] = (camera_forward_copy.n[0] * camera_to_unit_distance) + camera_x;
    camera_position.n[1] = (camera_forward_copy.n[1] * camera_to_unit_distance) + camera_y;
    camera_position.n[2] = (camera_forward_copy.n[2] * camera_to_unit_distance) + camera_z;

    collision_result collision;
    collision_test_vector(_collision_test_for_projectiles_flags /* 0x1000E9 */, &camera_position, &camera_forward_scaled, player->unit_index, &collision);

    real_vector3d impact_direction;
    impact_direction.n[0] = collision.point.n[0] - position->n[0];
    impact_direction.n[1] = collision.point.n[1] - position->n[1];
    impact_direction.n[2] = collision.point.n[2] - position->n[2];
    if ( normalize3d(&impact_direction) == 0.0f )
    {
        impact_direction.n[0] = direction->n[0];
        impact_direction.n[1] = direction->n[1];
        impact_direction.n[2] = direction->n[2];
    }

    if ( !hcex_off_aim_projectile )
    {
        real_vector3d blended_normal;
        fast_normals_interpolate(&impact_direction, &aim_direction, magnetism, &blended_normal);
        float cosine = (float)cos(aim_params.deviation_angle);
        float sine = (float)sin(aim_params.deviation_angle);
        pin_normal_to_cone3d(&blended_normal, direction, sine, cosine, direction);
    }

    player->aim_assist_unit_index = hit_object_index;
    player->aim_assist_timestamp = game_time_get();
    return hit_object_index;
}
