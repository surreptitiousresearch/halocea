/* action_vehicle_setup_specific @0x83821F70 — sets up an actor's "vehicle" action state for a specific
 * vehicle/seat: refuses if the actor is already in a vehicle (input.vehicle_index @344 != -1; field resolved
 * in the DB — the same offset appears in actor_combat_reaim_grenade.c/actor_look_update.c) or its offset-6 flag is
 * set, checks the vehicle is desirable to enter and has an entry animation for the seat, evaluates the seat
 * (any seat allowed, no optional out-references needed), finds a destination point/facing/surface near it,
 * and finally issues the move order. Returns 1 only if every step succeeds.
 *
 * DEVIATION — the DB's own decompile passes uninitialized locals `v10`/`v9` as action_vehicle_desirable's
 * trailing two params; disasm_range (0x83821FD0-0x83821FF0) shows both are actually just literal 0
 * (`li r9,0`/`li r8,0`) — FLT_MAX (`attempt_start_distance`/`attempt_continue_distance`) is loaded once into
 * f2 and copied to f1 (`fmr f1,f2`), matching the DB's own `action_vehicle_desirable` prototype exactly with
 * no scrambling.
 *
 * DEVIATION — the DB's `action_vehicle_find_destination` prototype lists `ignore_hint_reference`
 * (unsigned __int8*) before `destination_point`/`surface_index_reference` in argument order, but
 * disasm_range (0x83822064-0x83822088) shows the two pointers actually passed at that call (r8, r9) are sized
 * and offset exactly like `&state_data->destination_point` (real_point3d, offset 0x30) and
 * `&state_data->destination_surface_index` (int, offset 0x48) — matching `vehicle_state_data`'s real layout,
 * not a single-byte flag. r10 (where `ignore_hint_reference` would land next) is never set at this call site
 * (it still holds unrelated leftover arithmetic from the function's own prologue), so this call site only
 * meaningfully passes 7 arguments; declared that way here, matching the precedent set by
 * action_flee_find_flee_position's identical unset-trailing-register pattern. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/vehicle_state_data.h"
#include "headers/blam_data_globals.h"
extern void *memset(void *dst, int value, unsigned int n);

extern uint8_t action_vehicle_desirable(int actor_index, int vehicle_index, uint8_t scripted_request, float attempt_start_distance, float attempt_continue_distance, uint8_t already_inside, uint8_t already_attempting_entry);
extern uint8_t unit_has_animation_to_enter_seat(int unit_index, int vehicle_index, int16_t seat_index);
extern uint8_t action_vehicle_evaluate_seat(int actor_index, int vehicle_index, int16_t seat_index, uint8_t allow_any_seat, real_point3d *entry_point, real_vector3d *entry_facing, real_point3d *hint_point, float *seat_weight_reference, uint8_t *within_range_reference, uint8_t *correct_facing_reference, uint8_t *could_potentially_fake_reference);
extern uint8_t action_vehicle_find_destination(int actor_index, int vehicle_index, real_point3d *entry_point, real_vector3d *entry_facing, real_point3d *hint_point, real_point3d *destination_point, int *surface_index_reference);
extern uint8_t actor_move_to_point(int actor_index, real_point3d *destination, int surface_index, int ignore_target_object_index);

uint8_t action_vehicle_setup_specific(int actor_index, int vehicle_index, int16_t seat_index, vehicle_state_data *state_data)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    memset(state_data, 0, sizeof(vehicle_state_data));

    if (actor->input.vehicle_index != -1)
        return 0;

    if (actor->meta.swarm)
        return 0;

    if (!action_vehicle_desirable(actor_index, vehicle_index, 1u, 3.4028235e38f, 3.4028235e38f, 0, 0))
        return 0;

    state_data->vehicle_index = vehicle_index;
    state_data->seat_index = seat_index;
    state_data->impromptu = 0;

    if (!unit_has_animation_to_enter_seat(actor->meta.unit_index, vehicle_index, seat_index))
        return 0;

    real_point3d entry_point;
    real_vector3d entry_facing;
    if (!action_vehicle_evaluate_seat(actor_index, vehicle_index, seat_index, 1u, &entry_point, &entry_facing,
            0, 0, 0, 0, 0))
        return 0;

    if (!action_vehicle_find_destination(actor_index, vehicle_index, &entry_point, &entry_facing, 0,
            &state_data->destination_point, &state_data->destination_surface_index))
        return 0;

    if (!actor_move_to_point(actor_index, &state_data->destination_point, state_data->destination_surface_index,
            vehicle_index))
        return 0;

    return 1;
}
