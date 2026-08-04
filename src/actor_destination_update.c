/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* actor_destination_update @ 0x837CA680 — recompute the actor's immediate movement destination from its
 * current path. First refreshes the path (when the actor has a path request, is interesting this frame, and is
 * awake), then validates the destination. If a path is active, it walks the remaining waypoints to find which
 * one the actor is currently steering toward: a waypoint is "reached" either by simple proximity (when not yet
 * committed to a leg) or by a 2D segment-projection test that checks the actor has passed the segment's near
 * end while still facing along it. Once the target waypoint is chosen it emits the destination point
 * (actor+1292) and the destination delta vector (actor+1304 = dest - position). Special case: a directional-flying
 * vehicle driver (input.vehicle_driver_type == _actor_vehicle_driver_directional_flying) projects a fixed
 * 3-unit destination straight ahead along the facing vector, signed by
 * the steepness gate at actor+1516. Otherwise the destination is cleared.
 *
 * The path/guidance block lives at actor+1192; waypoints are a 16-byte-stride array whose position occupies the
 * first two floats (x,y) used for the 2D tests; the full xyz position sits at byte +32 within each point
 * (dword index +306 from the actor base for the selected point). Reads are by raw offset, matching the
 * decompiler. Position is actor+300 (float idx 75..77); facing is actor+372 (float idx 93..95). */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/path_state.h"
#include "headers/actor_vehicle_driver_type.h"
#include "headers/blam_data_globals.h"


#include "headers/path_state.h"
extern uint8_t actor_path_refresh(int actor_index, uint8_t new_destination, path_state *cached_path_state);
extern uint8_t actor_test_destination(uint16_t actor_index);
extern void actor_path_clear(uint16_t actor_index);

void actor_destination_update(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( actor->meta.timeslice && !actor->control.path.refreshed_this_tick && !actor->meta.dormant )      /* path requested, interesting this frame, not dormant */
        actor_path_refresh(actor_index, 0, NULL);

    actor_test_destination(actor_index);

    char *guidance = (char *)&actor->control.path.path;                       /* path/guidance block; [0] = path active */

    if ( actor->control.path.path.valid )
    {
        char reached;
        while ( 1 )
        {
            int  point_index = actor->control.path.path.step_index;              /* current waypoint index */
            char next_index  = point_index + 1;
            if ( point_index + 1 >= actor->control.path.path.step_count )        /* no further waypoints */
                goto resolve_destination;

            float *next_point = (float *)&actor->control.path.path.steps[point_index + 1].point;
            float *cur_point  = (float *)&actor->control.path.path.steps[point_index].point;
            reached = 0;

            if ( actor->control.movement_complete )                           /* leg already satisfied */
                goto mark_reached;

            if ( !actor->control.moving || !actor->control.movement_thwarted )
            {
                /* not committed to a leg: simple proximity to the current waypoint (radius 0.15) */
                float to_x = cur_point[0] - actor->input.position.body_position.x;
                float to_y = cur_point[1] - actor->input.position.body_position.y;
                reached = (to_y * to_y + to_x * to_x < 0.022500001f);
                goto evaluate;
            }

            /* committed: segment-projection test along waypoint[i] -> waypoint[i+1] */
            float seg_dx = next_point[0] - cur_point[0];
            float seg_dy = next_point[1] - cur_point[1];
            float near_dot = seg_dy * (cur_point[1] - actor->input.position.body_position.y)
                           + seg_dx * (cur_point[0] - actor->input.position.body_position.x);
            float facing_dot = actor->input.facing_vector.n[1] * seg_dy + actor->input.facing_vector.n[0] * seg_dx;
            if ( facing_dot > 0.0f && near_dot < 0.0f )
            {
                float t = -near_dot;
                float off_x = t * seg_dx + (cur_point[0] - actor->input.position.body_position.x);
                float off_y = seg_dy * t + (cur_point[1] - actor->input.position.body_position.y);
                reached = (off_y * off_y + off_x * off_x < 0.0625f);   /* radius 0.25 */
            }

evaluate:
            if ( !reached )
                goto resolve_destination;
            actor->control.path.path.step_index = next_index;                    /* advance to next waypoint and re-test */
            actor->control.movement_complete = 0;
            continue;

mark_reached:
            reached = 1;
            if ( !reached )                              /* unreachable; preserved to match decompiler flow */
                goto resolve_destination;
            actor->control.path.path.step_index = next_index;
            actor->control.movement_complete = 0;
        }

resolve_destination:
        if ( actor->control.movement_complete && actor->control.path.path.steps_finish_path )     /* satisfied final leg: stop here (path+0x18, was mis-named .flags) */
        {
            /* DEVIATION: collapsed inlined copy of actor_path_clear@0x837C86D0 (zero-xref) to its call. */
            actor_path_clear(actor_index);
        }
    }

    if ( actor->control.path.path.valid && (actor->control.moving || !actor->control.path.at_destination) )
    {
        /* destination = position of the current waypoint */
        /* BUGFIX vs prior transcription: the +306-int read was actor-relative (steps[idx].point),
         * not step-relative; the old code indexed 1224 bytes past the step. */
        path_step *step = &actor->control.path.path.steps[actor->control.path.path.step_index];
        actor->control.moving_towards_point.x = step->point.x;
        actor->control.moving_towards_point.y = step->point.y;
        actor->control.moving_towards_point.z = step->point.z;
        actor->control.moving_towards_vector.n[0] = actor->control.moving_towards_point.x - actor->input.position.body_position.x;
        actor->control.moving_towards_vector.n[1] = actor->control.moving_towards_point.y - actor->input.position.body_position.y;
        actor->control.moving_towards_vector.n[2] = actor->control.moving_towards_point.z - actor->input.position.body_position.z;
    }
    else if ( actor->input.vehicle_driver_type == _actor_vehicle_driver_directional_flying )   /* flying-vehicle driver: nudge 3 units straight ahead */
    {
        float sign = (actor->control.vector_avoidance_rotation_emergency_instantaneous > 0.89999998f) ? -1.0f : 1.0f;
        actor->control.movement_complete = 0;
        actor->control.moving = 1;
        float reach = sign * 3.0f;
        actor->control.moving_towards_vector.n[0] = actor->input.facing_vector.n[0] * reach;
        actor->control.moving_towards_vector.n[1] = actor->input.facing_vector.n[1] * reach;
        actor->control.moving_towards_vector.n[2] = actor->input.facing_vector.n[2] * reach;
        actor->control.moving_towards_point.x = actor->input.position.body_position.x + actor->control.moving_towards_vector.n[0];
        actor->control.moving_towards_point.y = actor->input.position.body_position.y + actor->control.moving_towards_vector.n[1];
        actor->control.moving_towards_point.z = actor->input.position.body_position.z + actor->control.moving_towards_vector.n[2];
    }
    else
    {
        actor->control.moving = 0;
        actor->control.movement_complete = 0;
        actor->control.path.at_destination = 1;
        /* DEVIATION: collapsed inlined copy of actor_path_clear@0x837C86D0 (zero-xref) to its call. */
        actor_path_clear(actor_index);
    }
}
