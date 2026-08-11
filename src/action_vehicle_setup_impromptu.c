/* action_vehicle_setup_impromptu @0x838225A8 — set up an actor's "impromptu" (unscripted) vehicle-entry
 * action: zero and prime the vehicle_state_data with the vehicle index and the attempt/continue
 * distances, then, if the actor isn't already in a vehicle, isn't part of a swarm, and isn't already
 * running the vehicle action (state.action == actor_action_vehicle), check whether the vehicle is desirable to enter and, if
 * so, find an impromptu seat, verify the unit has an entry animation for it, find a pathfinding
 * destination near the vehicle, and move the actor there. Returns whether the move was successfully
 * kicked off.
 *
 * DEVIATION (signature, disasm 0x838225A8-0x838225F8, 2026-07-31): five params, not seven. The args are
 * actor_index (r3), vehicle_index (r4), attempt_distance (f1), continue_distance (f2), state_data (r7).
 * The two float args f1/f2 each ALSO consume their GPR shadow slot (r5/r6), so r5/r6 are not separate
 * integer/pointer params: r5 is reloaded as the memset size literal and r6 is never read. state_data
 * (r7 = arg slot 4 — the output struct that is memset, filled field-by-field, and threaded through every
 * subsequent call) is therefore the 5th parameter, matching the DB prototype and the sole caller's
 * extern. The earlier 7-param reading mistook the f1/f2 GPR shadow slots for dead pointer/int params.
 * Field offsets on the state_data pointer in the decompile (+0,+4,+6,+0x20,+0x24,+0x30,+0x48) match
 * vehicle_state_data's layout exactly, so it's retyped and accessed by field here.
 *
 * DEVIATION: `action_vehicle_desirable`'s `already_inside`/`already_attempting_entry` arguments are shown
 * by the decompiler as uninitialized locals (v14/v15), but disasm loads both as literal 0 immediately
 * before the call — reproduced as literal 0, not uninitialized reads (same class as
 * find_position_for_ball.c).
 *
 * DEVIATION (2026-07-31): action_vehicle_find_destination takes 7 params (its own definition, disasm-
 * confirmed: r3-r9 used, r10 dead), not the 8 the stale DB prototype listed. The binary loads r10 with
 * unit_has_animation_to_enter_seat's masked result as a dead 8th arg, but the callee never reads r10, so
 * the extern and the call site here are synced to the real 7-param signature and that dead trailing arg
 * is dropped. */

#include <stdint.h>
#include <string.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/vehicle_state_data.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/actor_action.h"
#include "headers/blam_data_globals.h"



extern uint8_t action_vehicle_desirable(int actor_index, int vehicle_index, uint8_t scripted_request, float attempt_start_distance, float attempt_continue_distance, uint8_t already_inside, uint8_t already_attempting_entry);
extern int16_t action_vehicle_find_impromptu_seat(int actor_index, int vehicle_index, real_point3d *entry_point, real_vector3d *entry_facing, real_point3d *hint_point);
extern uint8_t unit_has_animation_to_enter_seat(int unit_index, int vehicle_index, int16_t seat_index);
extern uint8_t action_vehicle_find_destination(int actor_index, int vehicle_index, real_point3d *entry_point, real_vector3d *entry_facing, real_point3d *hint_point, real_point3d *destination_point, int *surface_index_reference);
extern uint8_t actor_move_to_point(int actor_index, real_point3d *destination, int surface_index, int ignore_target_object_index);

uint8_t action_vehicle_setup_impromptu(int actor_index, int vehicle_index, float attempt_distance, float continue_distance, vehicle_state_data *state_data)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    memset(state_data, 0, sizeof(vehicle_state_data));
    state_data->attempt_distance = attempt_distance;
    state_data->continue_distance = continue_distance;

    if ( actor->input.vehicle_index == -1 && !actor->meta.swarm && actor->state.action != actor_action_vehicle )
    {
        if ( action_vehicle_desirable(actor_index, vehicle_index, 0, attempt_distance, continue_distance, 0, 0) )
        {
            state_data->vehicle_index = vehicle_index;

            real_point3d entry_point;
            real_vector3d entry_facing_unused;
            real_vector3d shared_scratch; /* hint_point (call 1, uninitialized) then reused as entry_facing (call 2) */

            state_data->seat_index = action_vehicle_find_impromptu_seat(actor_index, vehicle_index, &entry_point,
                &entry_facing_unused, (real_point3d *)&shared_scratch);

            if ( state_data->seat_index != -1 )
            {
                state_data->impromptu = 1;

                int has_animation_to_enter_seat = unit_has_animation_to_enter_seat(actor->meta.unit_index,
                    vehicle_index, state_data->seat_index);

                if ( has_animation_to_enter_seat )
                {
                    /* r8 = destination_point (byte +0x30), r9 = destination_surface_index (byte +0x48):
                     * the callee's true arg5/arg6 (disasm-confirmed against its 7-param definition). The
                     * binary also loads r10 with has_animation_to_enter_seat, but that is the callee's dead
                     * r10 slot, so it is dropped here. */
                    if ( action_vehicle_find_destination(actor_index, vehicle_index, &entry_point,
                             &shared_scratch, 0, &state_data->destination_point,
                             &state_data->destination_surface_index) )
                    {
                        return actor_move_to_point(actor_index, &state_data->destination_point,
                            state_data->destination_surface_index, vehicle_index) != 0;
                    }
                }
            }
        }
    }

    return 0;
}
