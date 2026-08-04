/* action_vehicle_perform @0x838220C8 — per-tick update of an actor's in-progress "enter a vehicle" action.
 * The action state is the vehicle_state_data arm of the action union (target object index, seat index,
 * cached distances, timers, and result flags).
 *
 * Flow:
 *   - If the actor is already in a vehicle (unit-in-vehicle index at actor+0x158 != -1), mark the action
 *     resolved (actor+0xA5) and finish.
 *   - Otherwise, if the actor has not yet taken the seat (actor+0xA4 == 0):
 *       * Verify the target object still exists and is a vehicle (object_try_and_get_and_verify_type type 2).
 *         If it is gone, clear the target index and abort (actor+0xA6).
 *       * Re-check that the vehicle is still desirable; abort if not.
 *       * Stuck detection: every 150 ticks, if the actor has moved >5 units since the last check, reset the
 *         stuck counter (actor+0xAA) and re-cache its position; else bump the counter.
 *       * If the stuck counter is < 8, evaluate the seat. If the seat is claimable, decide whether the actor is
 *         within range and correctly facing (with a "fake it after 30 ticks" fallback); enter the seat when
 *         both hold, halt-and-align when in range but mis-facing, otherwise path toward the vehicle
 *         (find_destination + move_to_point, aborting after enough consecutive path failures).
 *   - Result: 1 while the action is live or resolved (actor+0xA5 / actor+0xA6), else 0.
 *
 * DEVIATION — dead-float-shadow-GPR: the DB decompile fabricates uninitialised trailing args at the two vehicle
 * helper call sites because the float params consume dead GPR shadows. Resolved via disasm:
 *   - action_vehicle_desirable (0x83822164-0x83822188): the true already_inside / already_attempting_entry are
 *     the constants li r8,0 / li r9,1 — this actor is mid-entry-attempt and not yet inside.
 *   - action_vehicle_evaluate_seat (0x8382222C-0x8382226C): the DB's phantom v23/v24/v25 out-pointers are really
 *     &within_range / &correct_facing / &could_potentially_fake (stack bytes at sp+0x71 / +0x70 / +0x72). The
 *     arg9<arg10<arg11 order (8-byte param slots at 0x54/0x5C/0x64) plus the post-call usage
 *     (in-range → if facing enter, else halt) fixes the assignment.
 *   - action_vehicle_find_destination (0x83822308-0x83822330): the DB's phantom 8th arg is dropped; its 7th arg
 *     is the int* surface_index_reference at actor+0xE4 (DB mis-casts it to real_point3d*), confirmed by the
 *     following actor_move_to_point(actor, &actor[0xCC], *(int*)&actor[0xE4], target_index). */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/vehicle_state_data.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern int game_time_get(void);
extern uint8_t action_vehicle_desirable(int actor_index, int vehicle_index, uint8_t scripted_request, float attempt_start_distance, float attempt_continue_distance, uint8_t already_inside, uint8_t already_attempting_entry);
extern uint8_t action_vehicle_evaluate_seat(int actor_index, int vehicle_index, int16_t seat_index, uint8_t allow_any_seat, real_point3d *entry_point, real_vector3d *entry_facing, real_point3d *hint_point, float *seat_weight_reference, uint8_t *within_range_reference, uint8_t *correct_facing_reference, uint8_t *could_potentially_fake_reference);
extern uint8_t action_vehicle_find_destination(int actor_index, int vehicle_index, real_point3d *entry_point, real_vector3d *entry_facing, real_point3d *hint_point, real_point3d *destination_point, int *surface_index_reference);
extern uint8_t unit_enter_seat(int unit_index, int parent_unit_index, int16_t seat_index);
extern uint8_t actor_move_halt(uint16_t actor_index);
extern uint8_t actor_move_to_point(int actor_index, real_point3d *destination, int surface_index, int ignore_target_object_index);

int action_vehicle_perform(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    vehicle_state_data *action_data = &actor->state.action_data.___u0.vehicle;

    int target_index = action_data->vehicle_index;                     /* vehicle object index being entered */
    __int16 seat_index = action_data->seat_index;
    unsigned __int8 scripted_entry = action_data->impromptu == 0;      /* actor+0xA2 == 0 -> forced/scripted entry */
    real_point3d *actor_position = &actor->input.position.body_position;

    void *vehicle_object = object_try_and_get_and_verify_type(target_index, object_mask_vehicle);

    if ( actor->input.vehicle_index != -1 )                            /* already seated in a vehicle */
    {
        action_data->vehicle_entry_done = 1;                           /* action resolved */
    }
    else if ( !action_data->started_entry )                            /* seat not yet taken */
    {
        if ( !vehicle_object )
        {
            action_data->vehicle_index = -1;
            action_data->vehicle_entry_failed = 1;                     /* abort */
        }
        else if ( !action_vehicle_desirable(actor_index, target_index, scripted_entry,
                        action_data->attempt_distance, action_data->continue_distance, 0, 1) )
        {
            action_data->vehicle_entry_failed = 1;                     /* no longer desirable -> abort */
        }
        else
        {
            int now = game_time_get();
            if ( now >= action_data->stuck_detection_time + 150 )      /* stuck check every 150 ticks */
            {
                action_data->stuck_detection_time = now;
                float dx = actor_position->n[0] - action_data->stuck_detection_point.n[0];
                float dz = actor_position->n[2] - action_data->stuck_detection_point.n[2];
                float dy = actor_position->n[1] - action_data->stuck_detection_point.n[1];
                if ( ((dz * dz) + ((dy * dy) + (dx * dx))) >= 25.0f )
                {
                    action_data->stuck_detection_counter = 0;          /* moved >5 units -> not stuck; re-cache position */
                    *(int *)&action_data->stuck_detection_point.n[0] = *(int *)&actor->input.position.body_position.n[0];
                    *(int *)&action_data->stuck_detection_point.n[1] = *(int *)&actor->input.position.body_position.n[1];
                    *(int *)&action_data->stuck_detection_point.n[2] = *(int *)&actor->input.position.body_position.n[2];
                }
                else
                {
                    ++action_data->stuck_detection_counter;
                }
            }

            real_point3d entry_point;
            real_vector3d entry_facing;
            real_point3d hint_point;
            unsigned __int8 within_range;
            unsigned __int8 correct_facing;
            unsigned __int8 could_potentially_fake;

            if ( action_data->stuck_detection_counter < 8
              && action_vehicle_evaluate_seat(actor_index, target_index, seat_index, scripted_entry,
                        &entry_point, &entry_facing, &hint_point, nullptr,
                        &within_range, &correct_facing, &could_potentially_fake) )
            {
                if ( could_potentially_fake )
                {
                    __int16 fake_ticks = action_data->fake_entry_potential_timer + 1;
                    action_data->fake_entry_potential_timer = fake_ticks;
                    if ( fake_ticks >= 30 )                    /* fake the entry after 30 ticks */
                    {
                        correct_facing = 1;
                        within_range = 1;
                    }
                }
                else
                {
                    action_data->fake_entry_potential_timer = 0;
                }

                if ( within_range )
                {
                    if ( correct_facing )
                    {
                        unit_enter_seat(actor->meta.unit_index, target_index, seat_index);
                        action_data->started_entry = 1;                /* seat taken */
                        goto finish_state;
                    }
                    actor_move_halt(actor_index);              /* in range but mis-facing: halt and align */
                    goto finish_state;
                }

                /* not within range yet: path toward the vehicle */
                if ( actor->meta.timeslice )
                {
                    unsigned __int8 moving = 0;
                    if ( action_vehicle_find_destination(actor_index, target_index, &entry_point, &entry_facing,
                                /* hint_point: reinterpret of vehicle_state_data.ignore_hint (byte +7), per decompile */
                                (real_point3d *)&action_data->ignore_hint, &action_data->destination_point,
                                &action_data->destination_surface_index) )
                    {
                        moving = actor_move_to_point(actor_index, &action_data->destination_point,
                                action_data->destination_surface_index, target_index) != 0;
                    }
                    if ( moving )
                    {
                        action_data->pathfinding_failures = 0;         /* reset path-fail counter */
                    }
                    else
                    {
                        __int16 fails = action_data->pathfinding_failures + 1;
                        action_data->pathfinding_failures = fails;
                        if ( fails > (scripted_entry ? 5 : 50) )
                            action_data->vehicle_entry_failed = 1;     /* too many path failures -> abort */
                    }
                }

finish_state:
                action_data->currently_correct_facing = correct_facing;
                action_data->currently_within_range = within_range;
                action_data->lock_facing =
                        (((entry_point.n[1] - actor->input.position.body_position.n[1])
                                        * (entry_point.n[1] - actor->input.position.body_position.n[1]))
                                + (((entry_point.n[2] - actor->input.position.body_position.n[2])
                                                * (entry_point.n[2] - actor->input.position.body_position.n[2]))
                                        + ((entry_point.n[0] - actor->input.position.body_position.n[0])
                                                * (entry_point.n[0] - actor->input.position.body_position.n[0])))) < 1.0f;
                action_data->destination_facing.n[0] = entry_facing.n[0];
                action_data->destination_facing.n[1] = entry_facing.n[1];
                action_data->destination_facing.n[2] = entry_facing.n[2];
            }
            else
            {
                action_data->vehicle_entry_failed = 1;                 /* seat unclaimable or stuck -> abort */
            }
        }
    }

    if ( action_data->vehicle_entry_done )
        return 1;
    if ( action_data->vehicle_entry_failed )
        return 1;
    return 0;
}
