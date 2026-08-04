#include <stdint.h>
#include "headers/collision_result.h"
#include "headers/object_type.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_euler_angles2d.h"
#include "headers/location.h"
#include "headers/sound_location.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/projectile_datum.h"
#include "headers/projectile_definition.h"
#include "headers/unit_datum.h"
#include "headers/player_datum.h"
#include "headers/object_flags.h"
#include "headers/projectile_datum_flags.h"
#include "headers/projectile_definition_flags.h"
#include "headers/game_difficulty_value.h"
#include "headers/blam_data_globals.h"
#include "headers/projectile_action.h"
#include "headers/periodic_function.h"
#include "headers/game_time_constants.h"
#include "headers/datum_index.h"

/* The projectile object datum (`projectile`, at object_header_data[index].datum) and its
   definition (`projectile_def`, via TAG_INSTANCE) are DB-typed struct accesses. The typed
   structs also cover the stack-local math (vectors, points, euler angles, collision results
   and the flyby sound location). Blam engine symbols are extern. */


extern double cos(double x);
extern double sin(double x);
extern float __fsqrts(float x);

extern void contrail_delete(int contrail_index);
extern void projectile_export_function_values(int projectile_index);
extern float game_difficulty_get_value(int16_t value_type);
extern void unit_get_center_of_mass(int unit_index, real_point3d *center_of_mass);
extern int game_time_get(void);
extern float periodic_function_evaluate(int16_t function_type, float time);
extern real_vector3d *vector3d_from_euler_angles2d(real_vector3d *vector, const real_euler_angles2d *angles);
extern float normalize3d(real_vector3d *v);
extern real_vector3d *perpendicular3d(const real_vector3d *a, real_vector3d *result);
extern void rotate_vector_about_axis(real_vector3d *v, const real_vector3d *n, float sine, float cosine);
extern uint8_t projectile_collision_test_line(int projectile_index, const real_point3d *new_position, collision_result *collision_result);
extern void projectile_collision(int projectile_index, collision_result *collision, real_point3d *new_position, real_vector3d *new_velocity, float time_left);
extern void ai_handle_spatial_effect(int object_index, const real_point3d *position, int16_t effect_type, int16_t volume, int16_t count);
extern int local_player_get_player_index(int16_t local_player_index);
extern float sound_definition_get_maximum_distance(int sound_definition_index);
extern void component_vectors_from_direction3d(const real_vector3d *vector, const real_vector3d *direction, real_vector3d *parallel, real_vector3d *perpendicular);
extern int unattached_impulse_sound_new(int definition_index, const sound_location *location, float scale, uint8_t is_player);
extern void object_translate(int object_index, const real_point3d *new_position, const location *new_location);
extern void object_compute_node_matrices(int object_index);
extern void contrail_owner_collision(uint16_t contrail_index, uint8_t object_dying, float dt);
extern void object_delete(int object_index);
extern void projectile_detonate(int projectile_index, uint8_t first_collision, float time_left);
extern void projectile_detonate_to_network(int object_index);

/* Per-frame update of a single flying projectile: applies homing/guidance toward a target,
   deceleration and gravity, sweeps the resulting motion for collisions (bouncing/penetrating up
   to 10 times per frame), plays flyby sounds for nearby local players, keeps the orientation
   basis aligned to velocity, then detonates or deletes the projectile once it comes to rest. */
uint8_t projectile_update(int projectile_index)
{
    projectile_datum *projectile =
        (projectile_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, projectile_index)->datum;
    projectile_definition *projectile_def =
        TAG_GET(projectile_definition, projectile->definition_index);

    __int16 collision_count = 0;
    char flyby_sound_played = 0;
    double time_remaining = 1.0;

    /* --- retire any contrail if the projectile has stopped emitting one --- */
    if ((projectile->projectile.flags & (1u << _projectile_tracer_bit)) == 0)
    {
        int contrail_slot = projectile->projectile.tracer_attachment_index_index;
        if (contrail_slot != -1)
        {
            int contrail_index = projectile->object.attachment_indices[contrail_slot];
            if (contrail_index != -1)
                contrail_delete(contrail_index);
            projectile->object.attachment_indices[projectile->projectile.tracer_attachment_index_index] = -1;
            projectile->projectile.tracer_attachment_index_index = -1;
        }
    }

    /* --- advance the projectile timers --- */
    projectile->projectile.arming_time = projectile->projectile.arming_time + projectile->projectile.arming_time_delta;
    projectile->projectile.deceleration_timer = (projectile->projectile.deceleration_timer_delta + projectile->projectile.deceleration_timer);

    /* --- decide whether the arming/age timer should advance this frame --- */
    {
        int detonation_timer_style = projectile_def->projectile.detonation_timer_mode;
        char should_age;
        int projectile_flags;

        if (detonation_timer_style == 1 || detonation_timer_style == 2)
            should_age = (projectile->projectile.flags >> _projectile_stopped_after_collision_bit) & 1;
        else
            should_age = 1;

        projectile_flags = projectile->projectile.flags;
        if ((projectile_flags & (1u << _projectile_counting_down_bit)) != 0
            || (projectile_flags & (1u << _projectile_attached_bit)) != 0)
            should_age = 1;

        if (should_age)
        {
            float age;
            if ((projectile_flags & (1u << _projectile_counting_down_bit)) == 0)
                projectile->projectile.flags = projectile_flags | (1u << _projectile_counting_down_bit);
            age = (projectile->projectile.detonation_timer_delta + projectile->projectile.detonation_timer);
            projectile->projectile.detonation_timer = projectile->projectile.detonation_timer_delta + projectile->projectile.detonation_timer;
            if (age >= 1.0)
            {
                projectile_datum *projectile_reload =
            (projectile_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, projectile_index)->datum;
                if (projectile_reload->projectile.action < _projectile_action_detonate)
                    projectile_reload->projectile.action = _projectile_action_detonate;
            }
        }
    }

    projectile_export_function_values(projectile_index);

    {
        const float flat_surface_z_threshold = 0.30000001f;

        while (( !projectile->projectile.action
                 || (projectile->projectile.action == _projectile_action_detonate && projectile->projectile.arming_time_delta != 0.0
                     && projectile->projectile.arming_time < 1.0))
               && (projectile->projectile.flags & (1u << _projectile_attached_bit)) == 0
               && (projectile->object.flags & (1u << _object_at_rest_bit)) == 0
               && projectile->object.parent_object_index == -1)
        {
            float *velocity = projectile->object.translational_velocity.n;
            float velocity_y = projectile->object.translational_velocity.n[1];
            float velocity_z = projectile->object.translational_velocity.n[2];
            float velocity_x = projectile->object.translational_velocity.n[0];
            char object_moved = 0;
            int target_object_index = projectile->projectile.target_object_index;
            int owner_object_index = projectile->projectile.ignore_object_index;
            float speed;
            double end_speed;
            double average_speed;
            real_vector3d working_velocity;
            real_point3d new_position;
            real_point3d aim_point;
            real_vector3d steering_axis;
            real_vector3d displacement;
            real_vector3d unit_velocity;
            real_euler_angles2d wander_angles;
            real_vector3d wander_direction;
            collision_result collision[2];
            float avg_velocity_x, avg_velocity_y, avg_velocity_z;
            float *position = projectile->object.position.n;
            float gravity_accel;

            working_velocity.n[0] = velocity_x;
            working_velocity.n[1] = velocity_y;
            working_velocity.n[2] = velocity_z;
            avg_velocity_z = projectile->object.translational_velocity.n[2];
            avg_velocity_y = velocity_y;
            avg_velocity_x = velocity_x;
            speed = __fsqrts(((velocity_z * velocity_z)
                           + ((velocity_x * velocity_x) + (velocity_y * velocity_y))));
            end_speed = speed;
            average_speed = speed;

            /* --- homing / guidance toward the target object --- */
            if (target_object_index != -1 && projectile_def->projectile.guided_angular_velocity > 0.0)
            {
                float turn_rate = (projectile_def->projectile.guided_angular_velocity * SECONDS_PER_TICK);
                unit_datum *target =
                    (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, target_object_index)->datum;
                float delta_z = (projectile->object.bounding_sphere_center.n[2] - target->object.bounding_sphere_center.n[2]);
                float delta_x = (projectile->object.bounding_sphere_center.n[0] - target->object.bounding_sphere_center.n[0]);
                float delta_y = (projectile->object.bounding_sphere_center.n[1] - target->object.bounding_sphere_center.n[1]);
                float distance_to_target;
                float wander_scale;
                int azimuth_seed;
                int elevation_seed;
                double cos_turn, sin_turn;
                float to_target_x, to_target_y, to_target_z;

                if (((1 << target->object.type) & object_mask_unit) != 0 && target->unit.player_index != -1)
                    turn_rate = (game_difficulty_get_value(_game_difficulty_projectile_guidance_vs_player_scale) * turn_rate);

                distance_to_target = __fsqrts(((delta_y * delta_y)
                                            + ((delta_x * delta_x) + (delta_z * delta_z))));
                if (distance_to_target <= 10.0)
                {
                    if (distance_to_target <= 2.0
                        || (wander_scale = ((distance_to_target - 2.0f) * 0.125f), wander_scale < 0.0))
                        wander_scale = 0.0f;
                    else if (wander_scale > 1.0)
                        wander_scale = 1.0f;
                }
                else
                {
                    wander_scale = 1.0f;
                }

                unit_get_center_of_mass(projectile->projectile.target_object_index, &aim_point);

                azimuth_seed = (unsigned __int16)(game_time_get() + 7 * (unsigned __int16)DATUM_INDEX_TO_IDENTIFIER(projectile_index));
                wander_angles.n[0] = (periodic_function_evaluate(_periodic_function_wander, ((float)azimuth_seed * 0.011111111f))
                                             * 6.2831855f);
                elevation_seed = (unsigned __int16)(game_time_get() + 3 * (unsigned __int16)DATUM_INDEX_TO_IDENTIFIER(projectile_index));
                wander_angles.n[1] = -((periodic_function_evaluate(_periodic_function_wander, ((float)elevation_seed * 0.011111111f))
                                                      * 1.5707964f) - 3.1415927f);
                vector3d_from_euler_angles2d(&wander_direction, &wander_angles);
                aim_point.n[0] = (wander_direction.n[0] * wander_scale) + aim_point.n[0];
                aim_point.n[1] = (wander_direction.n[1] * wander_scale) + aim_point.n[1];
                aim_point.n[2] = (wander_direction.n[2] * wander_scale) + aim_point.n[2];

                to_target_z = (aim_point.n[2] - projectile->object.position.n[2]);
                to_target_x = (aim_point.n[0] - projectile->object.position.n[0]);
                to_target_y = (aim_point.n[1] - projectile->object.position.n[1]);

                /* steering_axis = (aim - position) x velocity */
                steering_axis.n[2] = (to_target_y * projectile->object.translational_velocity.n[0])
                                   - (to_target_x * projectile->object.translational_velocity.n[1]);
                steering_axis.n[1] = (to_target_x * projectile->object.translational_velocity.n[2])
                                   - (to_target_z * projectile->object.translational_velocity.n[0]);
                steering_axis.n[0] = ((to_target_z * projectile->object.translational_velocity.n[1]))
                                   - (to_target_y * projectile->object.translational_velocity.n[2]);

                if (((to_target_z * projectile->object.translational_velocity.n[2])
                          + ((to_target_y * projectile->object.translational_velocity.n[1])
                                  + (projectile->object.translational_velocity.n[0] * to_target_x))) > 0.0
                    && normalize3d(&steering_axis) > 0.0)
                {
                    cos_turn = cos(turn_rate);
                    sin_turn = sin(turn_rate);
                    rotate_vector_about_axis(&working_velocity, &steering_axis, sin_turn, cos_turn);
                }
            }

            /* --- deceleration toward the projectile's final velocity --- */
            if (projectile->projectile.deceleration_timer < 1.0)
            {
                /* not yet fully spawned: keep the (possibly homing-adjusted) velocity */
                goto have_velocity_z;
            }
            else
            {
                float final_velocity = projectile_def->projectile.final_velocity;
                if (speed <= final_velocity || projectile->projectile.deceleration == 0.0)
                {
                    if (projectile_def->projectile.detonation_maximum_range == 0.0
                        && projectile_def->projectile.detonation_maximum_time == 0.0
                        && projectile_def->projectile.detonation_minimum_velocity <= final_velocity
                        && (projectile->projectile.deceleration != 0.0
                            || projectile->projectile.odometer >= (double)projectile->projectile.maximum_damage_distance))
                    {
                        projectile_datum *projectile_reload =
            (projectile_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, projectile_index)->datum;
                        if (projectile_reload->projectile.action < _projectile_action_disappear)
                            projectile_reload->projectile.action = _projectile_action_disappear;
                    }
                    else if (speed < final_velocity && speed > 0.0)
                    {
                        float clamp = ((projectile_def->projectile.final_velocity / speed) * 0.99000001f);
                        working_velocity.n[0] = clamp * working_velocity.n[0];
                        working_velocity.n[1] = clamp * working_velocity.n[1];
                        working_velocity.n[2] = clamp * working_velocity.n[2];
                        goto have_velocity_z;
                    }
                    /* otherwise fall through with the working velocity unchanged */
                }
                else
                {
                    float decel_amount = (projectile->projectile.deceleration * (float)time_remaining);
                    float old_velocity_z = working_velocity.n[2];
                    end_speed = (speed - decel_amount);
                    if (end_speed > final_velocity)
                    {
                        average_speed = -((decel_amount * 0.5f) - speed);
                        working_velocity.n[0] = ((speed - decel_amount) / speed) * working_velocity.n[0];
                        working_velocity.n[1] = ((speed - decel_amount) / speed) * working_velocity.n[1];
                        working_velocity.n[2] = ((float)end_speed / speed) * working_velocity.n[2];
                        avg_velocity_x = ((*velocity + working_velocity.n[0]) * 0.5f);
                        avg_velocity_y = (projectile->object.translational_velocity.n[1] + working_velocity.n[1]) * 0.5f;
                        avg_velocity_z = ((projectile->object.translational_velocity.n[2]
                                          + (((float)end_speed / speed) * old_velocity_z))) * 0.5f;
                    }
                    else
                    {
                        float phase = ((speed - projectile_def->projectile.final_velocity)
                                              / (projectile->projectile.deceleration * (float)time_remaining));
                        float clamped_final = (final_velocity * 0.99000001f);
                        float tail_velocity_z;
                        end_speed = (projectile_def->projectile.final_velocity * 0.99000001f);
                        working_velocity.n[2] = ((clamped_final / speed) * working_velocity.n[2]);
                        working_velocity.n[0] = ((clamped_final / speed)) * working_velocity.n[0];
                        working_velocity.n[1] = ((clamped_final / speed)) * working_velocity.n[1];
                        tail_velocity_z = (((float)1.0 - phase)
                                          * ((clamped_final / speed) * old_velocity_z));
                        average_speed = ((((clamped_final + speed) * phase) * 0.5f)
                                        + (((float)1.0 - phase) * final_velocity));
                        avg_velocity_x = (((*velocity + working_velocity.n[0]) * phase) * 0.5f)
                                       + (((float)1.0 - phase) * working_velocity.n[0]);
                        avg_velocity_y = (((projectile->object.translational_velocity.n[1] + working_velocity.n[1]) * phase) * 0.5f)
                                       + (((float)1.0 - phase) * working_velocity.n[1]);
                        avg_velocity_z = (((projectile->object.translational_velocity.n[2] + working_velocity.n[2]) * phase) * 0.5f)
                                       + tail_velocity_z;
                    }
                }
            }

have_velocity_z:
            /* --- gravity --- */
            {
                float gravity_scale = (projectile->object.flags & (1u << _object_wholly_under_media_bit))
                                    ? projectile_def->projectile.water_gravity_scale
                                    : projectile_def->projectile.air_gravity_scale;
                float gravity_delta;
                float maximum_range;
                float avg_velocity_z_gravity;
                float travel_fraction;
                float substep_time;

                gravity_accel = (gravity_scale * global_gravity);
                gravity_delta = (gravity_accel * (float)time_remaining);
                working_velocity.n[2] = working_velocity.n[2] - gravity_delta;
                maximum_range = projectile_def->projectile.detonation_maximum_range;
                avg_velocity_z_gravity = -((gravity_delta * 0.5f) - avg_velocity_z);

                if (maximum_range == 0.0
                    || (((float)average_speed * (float)time_remaining) + projectile->projectile.odometer) <= maximum_range)
                {
                    travel_fraction = 1.0f;
                }
                else
                {
                    projectile_datum *projectile_reload;
                    if (average_speed == 0.0)
                        travel_fraction = 0.0f;
                    else
                        travel_fraction = (((projectile_def->projectile.detonation_maximum_range
                                          - projectile->projectile.odometer) / (float)average_speed) * (float)time_remaining);
                    projectile_reload =
                        (projectile_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, projectile_index)->datum;
                    if (projectile_reload->projectile.action < _projectile_action_detonate)
                        projectile_reload->projectile.action = _projectile_action_detonate;
                }

                substep_time = (travel_fraction * (float)time_remaining);
                new_position.n[0] = (substep_time * avg_velocity_x) + projectile->object.position.n[0];
                new_position.n[1] = (substep_time * avg_velocity_y) + projectile->object.position.n[1];
                new_position.n[2] = (substep_time * avg_velocity_z_gravity) + projectile->object.position.n[2];

                if (collision_count == 10)
                    goto too_many_bounces;

                if (projectile->projectile.action != _projectile_action_disappear)
                {
                    object_moved = 1;
                    if (projectile_collision_test_line(projectile_index, &new_position, collision))
                    {
                        float velocity_z_at_impact;
                        time_remaining = ((float)1.0 - collision[0].t);
                        velocity_z_at_impact = ((gravity_accel * ((float)1.0 - collision[0].t))
                                               + working_velocity.n[2]);
                        working_velocity.n[2] = ((gravity_accel * ((float)1.0 - collision[0].t))
                                               + working_velocity.n[2]);
                        if (end_speed != 0.0)
                        {
                            float restored_speed = ((projectile->projectile.deceleration
                                                   * ((float)1.0 - collision[0].t)) + (float)end_speed);
                            float restore_scale;
                            if (restored_speed > speed)
                                restored_speed = speed;
                            restore_scale = (restored_speed / (float)end_speed);
                            working_velocity.n[0] = restore_scale * working_velocity.n[0];
                            working_velocity.n[1] = restore_scale * working_velocity.n[1];
                            working_velocity.n[2] = restore_scale * velocity_z_at_impact;
                        }
                        if (collision[0].plane.n.n[2] > (double)flat_surface_z_threshold)
                            projectile->projectile.flags |= (1u << _projectile_collided_once_bit);
                        projectile->projectile.ignore_object_index = -1;
                        /* time_left arg is unused by projectile_collision and not set at the call
                           site; pass the remaining frame time for a meaningful reconstruction */
                        projectile_collision(projectile_index, collision, &new_position, &working_velocity, (float)time_remaining);
                        ++collision_count;
                        ai_handle_spatial_effect(projectile_index, &collision[0].point, 1,
                                                 projectile_def->projectile.impact_noise, 1);
                        if ((projectile->projectile.flags & (1u << _projectile_attached_bit)) != 0)
                            object_moved = 0;
                        goto after_motion;
                    }
                }

                if (collision_count == 10)
                {
too_many_bounces:
                    {
                        projectile_datum *projectile_reload =
            (projectile_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, projectile_index)->datum;
                        if (projectile_reload->projectile.action < _projectile_action_detonate)
                            projectile_reload->projectile.action = _projectile_action_detonate;
                    }
                }
                time_remaining = 0.0;
            }

after_motion:
            if (object_moved)
            {
                displacement.n[0] = new_position.n[0] - *position;
                displacement.n[1] = new_position.n[1] - projectile->object.position.n[1];
                displacement.n[2] = new_position.n[2] - projectile->object.position.n[2];
                projectile->projectile.odometer = __fsqrts(((displacement.n[0] * displacement.n[0])
                                               + ((displacement.n[2] * displacement.n[2])
                                                       + (displacement.n[1] * displacement.n[1]))))
                                             + projectile->projectile.odometer;

                /* --- flyby sound for nearby local players --- */
                if (!flyby_sound_played && projectile_def->projectile.flyby_sound.index != -1)
                {
                    __int16 local_player;
                    for (local_player = 0; local_player < 2; ++local_player)
                    {
                        if (local_player_get_player_index(local_player) != -1)
                        {
                            int player_unit =
                                DATA_ARRAY_ELEMENT(player_data, player_datum, local_player_get_player_index(local_player))->unit_index;
                            if (player_unit != -1 && player_unit != owner_object_index)
                            {
                                float *unit_position =
                                    DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, player_unit)->datum->object.bounding_sphere_center.n;
                                float maximum_distance = sound_definition_get_maximum_distance(projectile_def->projectile.flyby_sound.index);
                                real_vector3d to_player;
                                real_vector3d along_travel;
                                real_vector3d perpendicular;
                                float along_distance;

                                to_player.n[0] = *unit_position - *position;
                                to_player.n[1] = unit_position[1] - projectile->object.position.n[1];
                                to_player.n[2] = unit_position[2] - projectile->object.position.n[2];
                                component_vectors_from_direction3d(&to_player, &displacement, &along_travel, &perpendicular);
                                along_distance = ((along_travel.n[0] * displacement.n[0])
                                               + ((along_travel.n[2] * displacement.n[2])
                                                       + (along_travel.n[1] * displacement.n[1])));
                                if (along_distance >= 0.0
                                    && along_distance < ((displacement.n[0] * displacement.n[0])
                                                       + ((displacement.n[2] * displacement.n[2])
                                                               + (displacement.n[1] * displacement.n[1])))
                                    && ((perpendicular.n[0] * perpendicular.n[0])
                                             + ((perpendicular.n[1] * perpendicular.n[1])
                                                     + (perpendicular.n[2] * perpendicular.n[2])))
                                       < (double)(maximum_distance * maximum_distance))
                                {
                                    sound_location flyby;
                                    location game_location;

                                    flyby.position.n[0] = *unit_position - perpendicular.n[0];
                                    flyby.position.n[1] = unit_position[1] - perpendicular.n[1];
                                    flyby.position.n[2] = unit_position[2] - perpendicular.n[2];
                                    flyby.forward = displacement;
                                    normalize3d(&flyby.forward);
                                    /* CAVEAT: overlapped register packing in the decompile — the emitter
                                       velocity is set to the zero vector and the game_location's cluster
                                       is copied from the collision result while leaf_index and is_player
                                       both come from the (zero) x-component bits, hence is_player == 0. */
                                    /* recovered: *(int*)&...cluster_index (4-byte copy) -> cluster_index + bonus members */
                                    game_location.cluster_index = collision[0].location.cluster_index;
                                    game_location.bonus         = collision[0].location.bonus;
                                    game_location.leaf_index = *(int *)&global_zero_vector3d->n[0];
                                    flyby.translational_velocity.n[2] = global_zero_vector3d->n[2];
                                    flyby.game_location = game_location;
                                    *(int *)&flyby.translational_velocity.n[0] = game_location.leaf_index;
                                    flyby.translational_velocity.n[1] = global_zero_vector3d->n[1];
                                    unattached_impulse_sound_new(projectile_def->projectile.flyby_sound.index, &flyby, 1.0f, 0);
                                    flyby_sound_played = 1;
                                }
                            }
                        }
                    }
                }

                /* --- keep the orientation basis aligned to velocity (or just spin) --- */
                if ((projectile_def->projectile.flags & (1u << _projectile_oriented_along_velocity_bit)) == 0
                    || (*velocity == 0.0 && projectile->object.translational_velocity.n[1] == 0.0 && projectile->object.translational_velocity.n[2] == 0.0))
                {
                    if ((projectile->projectile.flags & (1u << _projectile_has_nonzero_angular_velocity_bit)) != 0)
                    {
                        float forward_x, up_x;
                        float new_up_x, new_up_y, new_up_z;
                        float new_up_z_term;

                        rotate_vector_about_axis(&projectile->object.forward,
                                                 &projectile->projectile.rotation_axis,
                                                 projectile->projectile.rotation_sine, projectile->projectile.rotation_cosine);
                        rotate_vector_about_axis(&projectile->object.up,
                                                 &projectile->projectile.rotation_axis,
                                                 projectile->projectile.rotation_sine, projectile->projectile.rotation_cosine);
                        normalize3d(&projectile->object.forward);
                        up_x = projectile->object.forward.n[2];
                        forward_x = projectile->object.forward.n[0];
                        new_up_x = ((projectile->object.up.n[2] * projectile->object.forward.n[0])
                                 - (projectile->object.forward.n[2] * projectile->object.up.n[0]));
                        new_up_y = ((projectile->object.forward.n[2] * projectile->object.up.n[1])
                                 - (projectile->object.forward.n[1] * projectile->object.up.n[2]));
                        new_up_z = (((projectile->object.forward.n[1] * projectile->object.up.n[0])
                                        - (projectile->object.up.n[1] * projectile->object.forward.n[0]))
                                 * projectile->object.forward.n[0]);
                        new_up_z_term = (projectile->object.forward.n[1]
                                      * ((projectile->object.forward.n[2] * projectile->object.up.n[1])
                                              - (projectile->object.forward.n[1] * projectile->object.up.n[2])));
                        projectile->object.up.n[0] = (((projectile->object.forward.n[1] * projectile->object.up.n[0])
                                                              - (projectile->object.up.n[1] * projectile->object.forward.n[0]))
                                                       * projectile->object.forward.n[1])
                                                     - (((projectile->object.up.n[2] * projectile->object.forward.n[0])
                                                              - (projectile->object.forward.n[2] * projectile->object.up.n[0]))
                                                       * projectile->object.forward.n[2]);
                        projectile->object.up.n[1] = (up_x * new_up_y) - new_up_z;
                        projectile->object.up.n[2] = (new_up_x * forward_x) - new_up_z_term;
                        normalize3d(&projectile->object.up);
                    }
                }
                else
                {
                    unit_velocity.n[0] = *velocity;
                    unit_velocity.n[1] = projectile->object.translational_velocity.n[1];
                    unit_velocity.n[2] = projectile->object.translational_velocity.n[2];
                    if (normalize3d(&unit_velocity) > 0.0)
                    {
                        float forward_y = unit_velocity.n[1];
                        float forward_x = unit_velocity.n[0];
                        float up_y, up_z;
                        float new_up_x, new_up_y, new_up_z, new_up_x_term;

                        projectile->object.forward.n[2] = unit_velocity.n[2];
                        projectile->object.forward.n[0] = forward_x;
                        projectile->object.forward.n[1] = forward_y;
                        up_y = projectile->object.forward.n[1];
                        up_z = projectile->object.forward.n[2];
                        new_up_x = ((projectile->object.forward.n[2] * projectile->object.up.n[1])
                                 - (projectile->object.forward.n[1] * projectile->object.up.n[2]));
                        new_up_y = ((projectile->object.up.n[0] * projectile->object.forward.n[1])
                                 - (projectile->object.forward.n[0] * projectile->object.up.n[1]));
                        new_up_z = (((projectile->object.forward.n[0] * projectile->object.up.n[2])
                                        - (projectile->object.up.n[0] * projectile->object.forward.n[2]))
                                 * projectile->object.forward.n[2]);
                        new_up_x_term = (((projectile->object.up.n[0] * projectile->object.forward.n[1])
                                        - (projectile->object.forward.n[0] * projectile->object.up.n[1]))
                                 * projectile->object.forward.n[0]);
                        projectile->object.up.n[2] = (((projectile->object.forward.n[0] * projectile->object.up.n[2])
                                                              - (projectile->object.up.n[0] * projectile->object.forward.n[2]))
                                                       * projectile->object.forward.n[0])
                                                     - (((projectile->object.forward.n[2] * projectile->object.up.n[1])
                                                              - (projectile->object.forward.n[1] * projectile->object.up.n[2]))
                                                       * projectile->object.forward.n[1]);
                        projectile->object.up.n[0] = (new_up_y * up_y) - new_up_z;
                        projectile->object.up.n[1] = (new_up_x * up_z) - new_up_x_term;
                        if (normalize3d(&projectile->object.up) == 0.0)
                        {
                            real_vector3d *up = perpendicular3d(&projectile->object.forward,
                                                                &projectile->object.up);
                            normalize3d(up);
                        }
                    }
                    rotate_vector_about_axis(&projectile->object.up,
                                             &projectile->object.forward,
                                             projectile->projectile.rotation_sine, projectile->projectile.rotation_cosine);
                }

                object_translate(projectile_index, &new_position, &collision[0].location);
                *velocity = working_velocity.n[0];
                projectile->object.translational_velocity.n[1] = working_velocity.n[1];
                projectile->object.translational_velocity.n[2] = working_velocity.n[2];

                if (time_remaining != 0.0 && collision_count)
                {
                    int contrail_slot = projectile->projectile.tracer_attachment_index_index;
                    if (contrail_slot != -1 && projectile->object.attachment_indices[contrail_slot] != -1)
                    {
                        object_compute_node_matrices(projectile_index);
                        contrail_owner_collision(projectile->object.attachment_indices[projectile->projectile.tracer_attachment_index_index], 0,
                                                 (((float)1.0 - (float)time_remaining) * SECONDS_PER_TICK));
                    }
                }
            }

            if (time_remaining <= 0.0)
                break;
        }
    }

    /* --- resolve the projectile's requested action --- */
    {
        int action = projectile->projectile.action;
        if (action == 1)
        {
            if (projectile->projectile.arming_time_delta == 0.0 || projectile->projectile.arming_time >= 1.0)
            {
                /* datum_role is a 4-byte opaque NetworkedDatumRole; read exact bytes */
                int network_role = *(int *)&projectile->object.datum_role;
                if (network_role != 1)
                {
                    if (!network_role && projectile->projectile.replicate_detonation == 1)
                        projectile_detonate_to_network(projectile_index);
                    projectile_detonate(projectile_index, collision_count == 0, (float)time_remaining);
                    object_delete(projectile_index);
                }
            }
        }
        else if (action == 2)
        {
            object_delete(projectile_index);
        }
    }

    return 1;
}
