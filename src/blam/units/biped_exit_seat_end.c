/* biped_exit_seat_end @0x837AD690 — finish a biped's exit from a vehicle seat: flatten and re-normalize
 * its facing onto the horizontal plane (falling back to global forward if degenerate), reset its up
 * vector to global up, mark its object flags, and resolve its drop-out position against the old vehicle
 * via biped_fix_position. If the first (radius-only) placement fails, retry seeded with the old vehicle's
 * bounding-sphere center and radius.
 *
 * DEVIATION (corrected 2026-07-27): the flags arrive in r8/r9/r10 per biped_fix_position's own prologue
 * (r7 is the ABI's dead GPR slot for the float arg — an earlier note mistook stale r7 contents for
 * fix_below_new_position). Call 1 sets li r8,1 / li r9,0 / li r10,1 = (fix_below=1, dont_teleport=0,
 * use_radius_as_multiplier=1). Call 2 sets r8=1, r9=0; r10 is the reused (u8) of call 1's return value,
 * provably 0 on the only path that reaches call 2 = (1, 0, 0). */

#include <stdint.h>
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/biped_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/biped_datum_flags.h"
#include "headers/blam_data_globals.h"

extern float __fsqrts(float x);
extern float fabsf(float x);
extern uint8_t biped_fix_position(int biped_index, int line_of_site_object_index, const real_point3d *new_position, real_point3d *final_position, float max_radius_fudge_factor, uint8_t fix_below_new_position, uint8_t dont_teleport, uint8_t use_radius_as_multiplier);

void biped_exit_seat_end(int biped_index, int old_vehicle_index)
{
    biped_datum *biped = ((biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum);
    real_vector3d *facing = &biped->object.forward;

    /* flatten facing onto the horizontal plane, then normalize (or snap to global forward if degenerate) */
    facing->n[2] = 0.0f;
    float original_y = facing->n[1];
    float magnitude = __fsqrts((0.0f * 0.0f)
                               + (facing->n[0] * facing->n[0] + facing->n[1] * facing->n[1]));
    if (fabsf(magnitude) < 0.000099999997f)
    {
        facing->n[0] = global_forward3d->n[0];
        facing->n[1] = global_forward3d->n[1];
        facing->n[2] = global_forward3d->n[2];
    }
    else
    {
        float inv_magnitude = 1.0f / magnitude;
        facing->n[0] = facing->n[0] * inv_magnitude;
        facing->n[1] = original_y * inv_magnitude;
        facing->n[2] = 0.0f * inv_magnitude;
    }

    unsigned int flags = biped->biped.flags | (1u << _biped_airborne_bit);
    biped->object.up = *global_up3d;
    biped->biped.flags = flags;

    if (!biped_fix_position(biped_index, old_vehicle_index, nullptr, nullptr, 2.0f, 1u, 0, 1u))
    {
        vehicle_datum *old_vehicle = ((vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, old_vehicle_index)->datum);
        real_point3d exit_position;
        exit_position.n[0] = old_vehicle->object.bounding_sphere_center.n[0];
        exit_position.n[1] = old_vehicle->object.bounding_sphere_center.n[1];
        exit_position.n[2] = old_vehicle->object.bounding_sphere_center.n[2];
        biped_fix_position(biped_index, old_vehicle_index, &exit_position, nullptr,
                           old_vehicle->object.bounding_sphere_radius, 1u, 0, 0);
    }
}
