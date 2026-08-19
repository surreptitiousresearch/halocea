/* observer_postcheck @ 0x83710110 — clamp and finalize a local player's resolved camera, then publish it.
 * Steps: clamp focus distance and field-of-view to legal ranges; clamp the focus point to the +/-5000
 * world box; rotate the focus offset into the camera's heading and apply it; pull the eye back along the
 * forward vector by the (penetration-corrected) distance; resolve the eye's BSP location (precaching
 * resources when the cluster changes); nudge the eye out of shallow water; clamp the final eye position;
 * and copy the forward/up/fov plus negated integrator velocity into the result before handing it to the
 * renderer bridge (hcex_update_observer_res). */

#include <stdint.h>
#include "headers/observer_globals.h"
#include "headers/observer_command_flags.h"
#include "headers/structure_bsp.h"
#include "headers/structure_cluster.h"
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"

#include "headers/real_vector3d.h"
#include "headers/tag_block.h"
extern float  __fsqrts(float x);
extern double __fabs(double x);
extern void observer_check_penetration(real_point3d *focus_position, const real_vector3d *forward, const real_vector3d *up, float *distance, float safe_distance);
extern void scenario_location_from_point(location *location, const real_point3d *point);
extern void predicted_resources_precache(const tag_block *predicted_resources);
extern float scenario_location_water_depth(const location *location, const real_point3d *position);
extern void hcex_update_observer_res(observer_result *result);

static double clamp(double x, double lo, double hi)
{
    if ( x >= lo )
        return x <= hi ? x : hi;
    return lo;
}

void observer_postcheck(int16_t local_player_index)
{
    observer *obs = &observer_globals.local_players[local_player_index];
    location eye_location;
    float distance_f;                      /* DEVIATION: decompiler fused this float into eye_location.leaf_index (shared stack slot); untangled */
    real_point3d target;
    double distance, fov, heading_x, heading_y, len;
    double offset_x, offset_y;
    int suppress_penetration;
    float *result_pos;

    target.n[0] = obs->focus_position.n[0];
    target.n[1] = obs->focus_position.n[1];
    target.n[2] = obs->focus_position.n[2];

    distance = clamp(obs->focus_distance, 0.0, 3.4028235e38);
    distance_f = (float)distance;

    fov = obs->field_of_view;
    if ( fov >= 0.001 )
        fov = fov <= 1.5707964 ? fov : 1.5707964;
    else
        fov = 0.001;
    obs->field_of_view = (float)fov;

    target.n[0] = clamp(target.n[0], -5000.0, 5000.0);
    target.n[1] = clamp(target.n[1], -5000.0, 5000.0);
    target.n[2] = clamp(target.n[2], -5000.0, 5000.0);
    distance = clamp(distance, 0.0, 5000.0);
    distance_f = (float)distance;

    /* normalized heading from the forward vector's xy projection */
    heading_x = obs->forward.n[0];
    heading_y = obs->forward.n[1];
    len = __fsqrts(((obs->forward.n[1] * obs->forward.n[1])
                        + (obs->forward.n[0] * obs->forward.n[0])));
    if ( __fabs(len) >= 0.000099999997 )
    {
        double inv = 1.0f / (float)len;
        heading_x = (float)(inv * obs->forward.n[0]);
        heading_y = (float)(inv * obs->forward.n[1]);
    }

    suppress_penetration = obs->last_command.flags & (1u << _observer_command_ignore_obstructions_bit);
    offset_y = ((obs->focus_offset.n[0] * (float)heading_y)
                     - (obs->focus_offset.n[1] * (float)heading_x));
    offset_x = (((obs->focus_offset.n[0] * (float)heading_x)
                            + (obs->focus_offset.n[1] * (float)heading_y))
                     + target.n[0]);
    target.n[2] = obs->focus_offset.n[2] + target.n[2];
    target.n[0] = (float)offset_x;
    target.n[1] = (float)offset_y + target.n[1];

    if ( !suppress_penetration && distance != 0.0 )
    {
        observer_check_penetration(&target, &obs->forward, &obs->up, &distance_f, 0.02f);
        distance = distance_f;
    }

    result_pos = obs->result.position.n;
    obs->result.position.n[0] = -((obs->forward.n[0] * (float)distance) - target.n[0]);
    obs->result.position.n[1] = -((obs->forward.n[1] * (float)distance) - target.n[1]);
    obs->result.position.n[2] = -((obs->forward.n[2] * (float)distance) - target.n[2]);

    scenario_location_from_point(&eye_location, &obs->result.position);
    if ( eye_location.cluster_index != -1 )
    {
        if ( eye_location.cluster_index != obs->result.location.cluster_index )
            predicted_resources_precache(
                &((structure_cluster *)global_structure_bsp->clusters.address)[eye_location.cluster_index].predicted_resources);
        obs->result.location = eye_location;
    }

    {
        double water_depth = scenario_location_water_depth(&obs->result.location, &obs->result.position);
        if ( __fabs(water_depth) < 0.050000001 )
        {
            if ( water_depth <= 0.0 )
                obs->result.position.n[2] = (obs->result.position.n[2] + (float)water_depth) + 0.050000001f;
            else
                obs->result.position.n[2] = obs->result.position.n[2] - (0.050000001f - (float)water_depth);
        }
    }

    result_pos[0] = (float)clamp(result_pos[0], -5000.0, 5000.0);
    obs->result.position.n[1] = (float)clamp(obs->result.position.n[1], -5000.0, 5000.0);
    obs->result.position.n[2] = (float)clamp(obs->result.position.n[2], -5000.0, 5000.0);

    obs->result.velocity.n[0] = -obs->velocities.focus_position.n[0];
    obs->result.velocity.n[1] = -obs->velocities.focus_position.n[1];
    obs->result.velocity.n[2] = -obs->velocities.focus_position.n[2];
    obs->result.forward.n[0] = obs->forward.n[0];
    obs->result.forward.n[1] = obs->forward.n[1];
    obs->result.forward.n[2] = obs->forward.n[2];
    obs->result.up.n[0] = obs->up.n[0];
    obs->result.up.n[1] = obs->up.n[1];
    obs->result.up.n[2] = obs->up.n[2];
    obs->result.field_of_view = obs->field_of_view;

    hcex_update_observer_res(&obs->result);
}
