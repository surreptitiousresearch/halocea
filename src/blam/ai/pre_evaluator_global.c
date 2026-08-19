/* pre_evaluator_global @0x837EFE90 — the always-run firing-position pre-evaluator: applies the scoring biases
 * that are independent of the specific combat behaviour (path direction, preferred groups, avoid points, and
 * directional-driving heading). Runs two passes over the candidate firing positions:
 *
 *  1. Path-direction / clearance pass (when context->find_path_direction_from_actor): for each still-valid,
 *     non-discarded position it (a) adds up to 20 to the score when the position sits far enough off the
 *     actor's aim line, rejecting it outright when it is inside the clearance radius; (b) rejects positions
 *     whose path direction re-crosses the aim corridor near the actor. Accepted positions then receive the
 *     preferred-group bonus and, for each avoid point, a proximity bonus (up to 10) scaled by how far outside
 *     the avoid sphere they are.
 *  2. Directional-driving pass (when context->directional_driving and the actor is in a vehicle): scores each
 *     valid position by how well the direction to it aligns with the vehicle's facing — a full 15 straight
 *     ahead, tapering with the heading error, rejecting positions that require reversing/turning too sharply
 *     unless the vehicle is stopped/near, close by, or rejected positions are allowed.
 *
 * The actor datum is read at raw byte offsets (char *), per the pre_evaluator_* convention; the firing
 * positions and evaluation context use their modelled DB types. The referenced actor floats are all in the
 * opaque behaviour payload: +660 clearance radius, +688 aim-line start, +712 aim-line end, +724/+728 firing
 * reference radius pair, +732 firing reference point, +300 actor body position (float index 75), +344 vehicle
 * index (dword index 86).
 *
 * DEVIATION: several float comparisons use the shipped constant 0.000099999997 (the float encoding of 1e-4);
 * __fsqrts/__fabs kept as the single-precision intrinsics the disasm uses. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/firing_position.h"
#include "headers/firing_position_evaluation_context.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"


extern float __fsqrts(float x);
extern double __fabs(double x);
extern uint8_t actor_firing_position_discarded(int actor_index, int16_t firing_position_index);
extern float point_to_line_distance_squared3d(const real_point3d *point, const real_point3d *base, const real_vector3d *height);
extern float vector_to_line_distance_squared3d(const real_point3d *point, const real_vector3d *vector, const real_point3d *base, const real_vector3d *height);
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);

void pre_evaluator_global(int actor_index, firing_position_evaluation_context *evaluation_context, int16_t firing_position_count, firing_position *firing_positions)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    const real_point3d *aim_line_start = &actor->danger_zone.position; /* recovered: @688 is danger_zone.position (was mislabeled initial_position) */
    const real_point3d *actor_body_position = &actor->input.position.body_position; /* &actor[75] == +300 */
    float clearance_radius = actor->danger_zone.danger_radius; /* recovered: @660 is danger_zone.danger_radius (was mislabeled position.z) */

    for ( int i = 0; i < firing_position_count; ++i )
    {
        firing_position *position = &firing_positions[i];
        if ( !position->valid )
            continue;

        uint8_t proceed;
        if ( !actor_firing_position_discarded(actor_index, position->original_index) )
        {
            proceed = 1;
        }
        else
        {
            position->rejected = 1;
            proceed = evaluation_context->allow_rejected_positions;
        }
        if ( !proceed )
        {
            position->valid = 0;
            continue;
        }

        if ( evaluation_context->find_path_direction_from_actor )
        {
            const real_point3d *candidate = &position->definition->position;

            /* aim-line vector (aim-line end minus start) */
            real_vector3d aim_line_delta;
            /* recovered: @712-@688 -> predict_danger_position - position (was time_left/bsp_leaves_contacted/passability_degraded_count - initial_position) */
            aim_line_delta.n[0] = actor->danger_zone.predict_danger_position.__s1.x - actor->danger_zone.position.__s1.x;
            aim_line_delta.n[1] = actor->danger_zone.predict_danger_position.__s1.y - actor->danger_zone.position.__s1.y;
            aim_line_delta.n[2] = actor->danger_zone.predict_danger_position.__s1.z - actor->danger_zone.position.__s1.z;

            /* clearance push: only when the candidate is near the firing reference point */
            /* recovered: +732/+736/+740 -> danger_zone.bounding_sphere_center.__s1.x/y/z, +728 -> bounding_sphere_radius */
            float dx = (candidate->n[0] - actor->danger_zone.bounding_sphere_center.__s1.x);
            float dy = (candidate->n[1] - actor->danger_zone.bounding_sphere_center.__s1.y);
            float dz = (candidate->n[2] - actor->danger_zone.bounding_sphere_center.__s1.z);
            float reference_reach = (actor->danger_zone.bounding_sphere_radius + (float)2.5);
            if ( ((dx * dx)
                       + ((dz * dz) + (dy * dy)))
                    < (double)(reference_reach * reference_reach) )
            {
                float line_distance_squared =
                        point_to_line_distance_squared3d(candidate, aim_line_start, &aim_line_delta);
                float clearance_bonus = 0.0f;
                if ( line_distance_squared >= (clearance_radius * clearance_radius) )
                {
                    float clearance_reach = (clearance_radius + (float)2.5);
                    if ( line_distance_squared >= (clearance_reach * clearance_reach) )
                        clearance_bonus = 20.0f;
                    else
                        clearance_bonus = ((__fsqrts(line_distance_squared)
                                - clearance_radius) * (float)8.0);
                }
                else
                {
                    /* candidate sits inside the aim-line clearance radius -> reject */
                    position->rejected = 1;
                    if ( !evaluation_context->allow_rejected_positions )
                    {
                        position->valid = 0;
                        goto apply_position_weighting_skip;
                    }
                }
                position->evaluation = position->evaluation + clearance_bonus;
            }

            /* path-corridor re-crossing test near the actor's body position */
            /* recovered: +732/+736/+740 -> danger_zone.bounding_sphere_center.__s1.x/y/z, +728 -> bounding_sphere_radius */
            float body_dx = (actor->input.position.body_position.__s1.x - actor->danger_zone.bounding_sphere_center.__s1.x);
            float body_dy = (actor->input.position.body_position.__s1.y - actor->danger_zone.bounding_sphere_center.__s1.y);
            float body_dz = (actor->input.position.body_position.__s1.z - actor->danger_zone.bounding_sphere_center.__s1.z);
            float corridor_reach = (actor->danger_zone.bounding_sphere_radius + (float)3.0);
            if ( ((body_dx * body_dx)
                       + ((body_dz * body_dz) + (body_dy * body_dy)))
                    >= (double)(corridor_reach * corridor_reach) )
                goto apply_position_weighting;
            if ( actor->danger_zone.current_distance_from_actor <= (double)clearance_radius ) /* recovered: +724 -> danger_zone.current_distance_from_actor */
                goto apply_position_weighting;
            if ( point_to_line_distance_squared3d(actor_body_position, aim_line_start, &aim_line_delta)
                    <= (double)(clearance_radius * clearance_radius) )
                goto apply_position_weighting;

            real_vector3d path_direction_scaled;
            path_direction_scaled.n[0] = position->path_direction_from_actor.n[0] * (float)3.0;
            path_direction_scaled.n[1] = position->path_direction_from_actor.n[1] * (float)3.0;
            path_direction_scaled.n[2] = position->path_direction_from_actor.n[2] * (float)3.0;
            if ( ((path_direction_scaled.n[0] * path_direction_scaled.n[0])
                       + ((path_direction_scaled.n[2] * path_direction_scaled.n[2])
                               + (path_direction_scaled.n[1] * path_direction_scaled.n[1])))
                    <= 0.000099999997
              || vector_to_line_distance_squared3d(actor_body_position, &path_direction_scaled, aim_line_start,
                         &aim_line_delta) >= (double)(clearance_radius * clearance_radius)
              || (position->rejected = 1, evaluation_context->allow_rejected_positions) )
            {
                goto apply_position_weighting;
            }
            position->valid = 0;
            goto apply_position_weighting_skip;
        }

apply_position_weighting:
        {
            firing_position_definition *definition = position->definition;
            if ( ((1 << definition->group_index) & evaluation_context->preferred_groups) != 0 )
                position->evaluation = evaluation_context->preferred_weight + position->evaluation;

            int avoid_point_count = evaluation_context->avoid_point_count;
            if ( avoid_point_count > 0 )
            {
                float closest_ratio = 1.0f;
                for ( int j = 0; j < avoid_point_count; ++j )
                {
                    firing_position_avoid_point *avoid = &evaluation_context->avoid_point[j];
                    float adx = (avoid->point.n[0] - definition->position.n[0]);
                    float ady = (avoid->point.n[1] - definition->position.n[1]);
                    float adz = (avoid->point.n[2] - definition->position.n[2]);
                    float ratio = (((adx * adx)
                                    + ((adz * adz) + (ady * ady)))
                                / (avoid->radius * avoid->radius));
                    if ( ratio < closest_ratio )
                        closest_ratio = ratio;
                }
                float avoid_bonus = 10.0f;
                if ( closest_ratio < 1.0f )
                    avoid_bonus = (__fsqrts(closest_ratio) * (float)10.0);
                position->evaluation = position->evaluation + avoid_bonus;
            }
        }
apply_position_weighting_skip:
        ;
    }

    if ( !evaluation_context->directional_driving )
        return;

    int vehicle_index = actor->input.vehicle_index;
    if ( vehicle_index == -1 )
        return;

    object_datum *vehicle_object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum;
    real_point3d vehicle_origin;
    object_get_origin(vehicle_index, &vehicle_origin);

    for ( int i = 0; i < firing_position_count; ++i )
    {
        firing_position *position = &firing_positions[i];
        if ( !position->valid )
            continue;

        const real_point3d *candidate = &position->definition->position;
        float dx = (candidate->n[0] - vehicle_origin.n[0]);
        float dy = (candidate->n[1] - vehicle_origin.n[1]);
        float dz = (candidate->n[2] - vehicle_origin.n[2]);
        float distance_squared = ((dx * dx)
                + ((dz * dz) + (dy * dy)));
        if ( __fabs(distance_squared) < 0.000099999997 || distance_squared >= 900.0 )
            continue;

        /* heading alignment: dot(vehicle_forward, unit direction to candidate) */
        float heading_alignment = (((vehicle_object->object.forward.n[0] * dx)
                        + ((vehicle_object->object.forward.n[2] * dz)
                                + (vehicle_object->object.forward.n[1] * dy)))
                / __fsqrts(distance_squared));

        float vehicle_speed_squared = ((vehicle_object->object.translational_velocity.n[2]
                        * vehicle_object->object.translational_velocity.n[2])
                + ((vehicle_object->object.translational_velocity.n[0] * vehicle_object->object.translational_velocity.n[0])
                        + (vehicle_object->object.translational_velocity.n[1] * vehicle_object->object.translational_velocity.n[1])));
        if ( (!evaluation_context->directional_driving_cannot_stop && vehicle_speed_squared <= 0.0069444445)
          || distance_squared >= 64.0
          || heading_alignment >= 0.70710677
          || (position->rejected = 1, evaluation_context->allow_rejected_positions) )
        {
            float heading_bonus = 15.0f;
            if ( heading_alignment >= 0.0f )
            {
                if ( heading_alignment > 0.86602539 )
                    heading_bonus = (((heading_alignment - (float)0.86602539)
                            * (float)111.96151) + (float)15.0);
            }
            else
            {
                heading_bonus = -((heading_alignment * (float)-15.0) - (float)15.0);
            }
            position->evaluation = position->evaluation + heading_bonus;
        }
        else
        {
            position->valid = 0;
        }
    }
}
