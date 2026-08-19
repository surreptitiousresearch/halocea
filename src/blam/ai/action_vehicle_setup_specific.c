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
 * DEVIATION — the DB's `action_vehicle_find_destination` prototype is one parameter too long from slot 4 on
 * (a phantom `entry_facing` at r6 shifts every later name one register right), so its `ignore_hint_reference`
 * appears before `destination_point`/`surface_index_reference` in argument order. disasm_range
 * (0x83822064-0x83822088) shows the two pointers actually passed at that call (r8, r9) are sized and offset
 * exactly like `&state_data->destination_point` (real_point3d, offset 0x30) and
 * `&state_data->destination_surface_index` (int, offset 0x48) — matching `vehicle_state_data`'s real layout,
 * not a single-byte flag. r10 (where the DB's 8th param would land next) is never set at this call site
 * (it still holds unrelated leftover arithmetic from the function's own prologue), so this call site only
 * meaningfully passes 7 arguments; declared that way here, matching the precedent set by
 * action_flee_find_flee_position's identical unset-trailing-register pattern.
 *
 * DEVIATION — the binary hands action_vehicle_find_destination's 4th argument the SAME frame buffer that
 * action_vehicle_evaluate_seat filled as `hint_point` — `addi r6, r1, 0xE0+var_70` @0x83822078 is the r1+0x70
 * slot of `addi r9, r1, 0xE0+var_70` @0x83822030 — while the entry_facing buffer (r1+0x90) is never read
 * again. The callee's own body consumes that parameter as a POINT (three `lfs` loads, then the anchor select
 * `mr r11, r6` @0x83821C44), so its declaration has been corrected to `const real_point3d *hint_point` and the
 * call below now spells the argument `&hint_point`. `entry_facing` remains an evaluate_seat out-parameter this
 * function requests and does not otherwise use. */

#include <stdint.h>
#include <string.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/vehicle_state_data.h"
#include "headers/blam_data_globals.h"

extern uint8_t action_vehicle_desirable(int actor_index, int vehicle_index, uint8_t scripted_request, float attempt_start_distance, float attempt_continue_distance, uint8_t already_inside, uint8_t already_attempting_entry);
extern uint8_t unit_has_animation_to_enter_seat(int unit_index, int vehicle_index, int16_t seat_index);
extern uint8_t action_vehicle_evaluate_seat(int actor_index, int vehicle_index, int16_t seat_index, uint8_t allow_any_seat, real_point3d *entry_point, real_vector3d *entry_facing, real_point3d *hint_point, float *seat_weight_reference, uint8_t *within_range_reference, uint8_t *correct_facing_reference, uint8_t *could_potentially_fake_reference);
extern uint8_t action_vehicle_find_destination(int actor_index, int vehicle_index, real_point3d *entry_point, const real_point3d *hint_point, uint8_t *ignore_hint_reference, real_point3d *destination_point, int *surface_index_reference);
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
    real_point3d hint_point;
    /* DEVIATION: the hint_point out-parameter is a real frame buffer, not NULL — `addi r9, r1, 0xE0+var_70`
     * @0x83822030 sets the r9 slot (allow_any_seat is r6, entry_point r7, entry_facing r8, hint_point r9,
     * seat_weight_reference r10, and the three trailing byte references are the stack words at 0x54/0x5C/0x64
     * that this site writes 0 to). action_vehicle_evaluate_seat itself fills it from
     * unit_get_seat_entrance_point's hint point (`stw` triple through r17 @0x83821A54-0x83821A68). */
    if (!action_vehicle_evaluate_seat(actor_index, vehicle_index, seat_index, 1u, &entry_point, &entry_facing,
            &hint_point, 0, 0, 0, 0))
        return 0;

    if (!action_vehicle_find_destination(actor_index, vehicle_index, &entry_point, &hint_point, 0,
            &state_data->destination_point, &state_data->destination_surface_index))
        return 0;

    if (!actor_move_to_point(actor_index, &state_data->destination_point, state_data->destination_surface_index,
            vehicle_index))
        return 0;

    return 1;
}
