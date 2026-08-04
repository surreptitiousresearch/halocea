/* local_player_aim_assist @0x836C675C — compute the aim-assist result for a local player this frame:
 * the autoaim and magnetism strengths, the angular position of the best target relative to the camera,
 * and that target's angular velocity (so the reticle can lead a moving target). Returns the target object
 * index, or -1 when there is no valid target or the player is not in a first/third-person view.
 *
 * The angular velocity is the target's velocity relative to the aiming unit, decomposed into yaw and
 * pitch rates about the aim direction. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/observer_result.h"
#include "headers/aim_assist_parameters.h"
#include "headers/aim_assist_target.h"
#include "headers/real_euler_angles2d.h"
#include "headers/real_vector3d.h"
#include "headers/player_datum.h"
#include "headers/director_perspective.h"
#include "headers/blam_data_globals.h"


#include "headers/real_point3d.h"
extern int16_t director_get_perspective(int16_t local_player_index);
extern int local_player_get_player_index(int16_t local_player_index);
extern int unit_get_aiming_unit_index(int unit_index);
extern int16_t player_control_get_zoom_level(int16_t local_player_index);
extern uint8_t unit_get_aim_assist_parameters(int unit_index, int16_t zoom_level, aim_assist_parameters *parameters);
extern const observer_result *observer_get_camera(int16_t local_player_index);
extern uint8_t aim_assist(const aim_assist_parameters *parameters, const real_point3d *position, const real_vector3d *direction, int ignore_object_index, int16_t ignore_team_index, aim_assist_target *target);
extern real_euler_angles2d *euler_angles2d_from_vector3d(real_euler_angles2d *angles, const real_vector3d *vector);
extern void object_get_velocities(int object_index, real_vector3d *translational_velocity, real_vector3d *angular_velocity);
extern float __fsqrts(float x);

int local_player_aim_assist(int16_t local_player_index, float *autoaim_level, float *magnetism_level,
                            real_euler_angles2d *target_angular_position,
                            real_euler_angles2d *target_angular_velocity)
{
    __int16 perspective = director_get_perspective(local_player_index);
    *autoaim_level = 0.0f;
    *magnetism_level = 0.0f;
    target_angular_position->n[1] = 0.0f;
    target_angular_position->n[0] = 0.0f;
    target_angular_velocity->n[0] = 0.0f;
    target_angular_velocity->n[1] = 0.0f;

    if (perspective != _director_perspective_first_person && perspective != _director_perspective_third_person)
        return -1;

    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum,
                 local_player_get_player_index(local_player_index));
    int aiming_unit_index = unit_get_aiming_unit_index(player->unit_index);
    __int16 zoom_level = player_control_get_zoom_level(local_player_index);

    aim_assist_parameters parameters;
    if (!unit_get_aim_assist_parameters(aiming_unit_index, zoom_level, &parameters))
        return -1;

    const observer_result *camera = observer_get_camera(local_player_index);
    aim_assist_target target[2];
    if (!aim_assist(&parameters, &camera->position, &camera->forward, aiming_unit_index,
                    player->team_index, target))
        return -1;

    *autoaim_level = target[0].autoaim_level;
    *magnetism_level = target[0].magnetism_level;
    euler_angles2d_from_vector3d(target_angular_position, &target[0].vector);

    real_vector3d aiming_velocity;
    real_vector3d target_velocity;
    object_get_velocities(player->unit_index, &aiming_velocity, nullptr);
    object_get_velocities(target[0].object_index, &target_velocity, nullptr);
    int result = target[0].object_index;

    float direction_z = target[0].vector.n[2];
    float relative_z = (target_velocity.n[2] - aiming_velocity.n[2]);
    float horizontal_magnitude = __fsqrts((target[0].vector.n[0] * target[0].vector.n[0])
                                        + (target[0].vector.n[1] * target[0].vector.n[1]));
    float full_magnitude_sq = (target[0].vector.n[2] * target[0].vector.n[2])
                            + ((target[0].vector.n[0] * target[0].vector.n[0])
                                    + (target[0].vector.n[1] * target[0].vector.n[1]));
    float radial_rate = ((target[0].vector.n[0] * (target_velocity.n[0] - aiming_velocity.n[0]))
                              + (target[0].vector.n[1] * (target_velocity.n[1] - aiming_velocity.n[1])))
                      / horizontal_magnitude;

    target_angular_velocity->n[0] =
        ((target[0].vector.n[0] * (target_velocity.n[1] - aiming_velocity.n[1]))
              - (target[0].vector.n[1] * (target_velocity.n[0] - aiming_velocity.n[0])))
        / ((target[0].vector.n[0] * target[0].vector.n[0])
                + (target[0].vector.n[1] * target[0].vector.n[1]));

    target_angular_velocity->n[1] =
        ((relative_z * horizontal_magnitude) - (radial_rate * direction_z))
        / full_magnitude_sq;

    return result;
}
