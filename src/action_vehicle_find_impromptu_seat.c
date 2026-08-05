/* action_vehicle_find_impromptu_seat @0x83822420 — scan every seat of `vehicle_index`'s tag definition,
 * evaluating each with action_vehicle_evaluate_seat (allow_any_seat=0, out-flags not needed so passed NULL),
 * and keep the one with the highest returned seat_weight (strictly greater than the running best, which
 * starts at 0.0). Writes the winning entry_point/entry_facing/hint_point through the optional out-pointers
 * and returns the winning seat index, or -1 if no seat qualifies.
 *
 * DEVIATION: disasm_range(0x83822420,0x838225A0) resolves what Hex-Rays flagged as a failed local variable
 * allocation. The three trailing action_vehicle_evaluate_seat out-params
 * (within_range_reference/correct_facing_reference/could_potentially_fake_reference) are confirmed passed as
 * literal 0 (NULL) at this call site: the caller stores 0 to its own stack offsets 0x54/0x5C/0x64, which are
 * exactly the stack-arg offsets action_vehicle_evaluate_seat.c's own header comment documents reading its
 * 9th-11th params from. The decompiler's `v21`/`v22`/`v23` (declared but never assigned) were Hex-Rays simply
 * failing to bind those NULL stack stores back to the call.
 *
 * FAITHFUL: if no seat is ever evaluated as valid (vehicle has zero seats, or every candidate seat fails),
 * best_entry_point/best_entry_facing/best_hint_point are never written by the loop and are read out
 * uninitialized — reproduced as genuinely uninitialized locals per the "don't invent" doctrine, since the
 * caller only trusts these outputs when the returned seat index is not -1. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/vehicle_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"


extern uint8_t action_vehicle_evaluate_seat(int actor_index, int vehicle_index, int16_t seat_index, uint8_t allow_any_seat, real_point3d *entry_point, real_vector3d *entry_facing, real_point3d *hint_point, float *seat_weight_reference, uint8_t *within_range_reference, uint8_t *correct_facing_reference, uint8_t *could_potentially_fake_reference);

int16_t action_vehicle_find_impromptu_seat(int actor_index, int vehicle_index, real_point3d *entry_point, real_vector3d *entry_facing, real_point3d *hint_point)
{
    real_point3d best_entry_point;
    real_vector3d best_entry_facing;
    real_point3d best_hint_point;
    int best_seat_index = -1;
    float best_seat_weight = 0.0f;

    object_datum *vehicle = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum);
    vehicle_definition *vehicle_def = TAG_GET(vehicle_definition, vehicle->definition_index);
    int seat_count = vehicle_def->unit.seats.count; /* unit.seats.count, abs +0x2E4 */

    for ( int16_t seat_index = 0; seat_index < seat_count; seat_index = (int16_t)(seat_index + 1) )
    {
        float seat_weight;
        real_point3d candidate_entry_point;
        real_vector3d candidate_entry_facing;
        real_point3d candidate_hint_point;

        if ( action_vehicle_evaluate_seat(actor_index, vehicle_index, seat_index, 0, &candidate_entry_point,
                &candidate_entry_facing, &candidate_hint_point, &seat_weight, 0, 0, 0)
          && seat_weight > best_seat_weight )
        {
            best_seat_index = seat_index;
            best_entry_point = candidate_entry_point;
            best_seat_weight = seat_weight;
            best_entry_facing = candidate_entry_facing;
            best_hint_point = candidate_hint_point;
        }
    }

    if ( entry_point )
        *entry_point = best_entry_point;
    if ( entry_facing )
        *entry_facing = best_entry_facing;
    int result = best_seat_index;
    if ( hint_point )
        *hint_point = best_hint_point;

    return result;
}
