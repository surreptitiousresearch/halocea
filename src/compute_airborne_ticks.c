/* compute_airborne_ticks @0x83760530 — per-tick airborne bookkeeping for a vehicle. Walks the
 * vehicle's mass points: while no mass point reports ground contact the vehicle is airborne and its
 * airborne_ticks counter advances; a contact/grounded flag resets airborne_ticks and advances
 * on_ground_ticks.
 *
 * DEVIATION: the decompiler invents a third param and mislabels the mass-point array as
 * powered_mass_point_datum. The caller's extern is the truth — two args, the array is
 * mass_point_datum (304-byte stride, matching the observed 0x130 element stride).
 *
 * DEVIATION: the prior draft's byte-offset macros were name-inverted vs the DB. vehicle_datum+0x4D0
 * is `vehicle.airborne_ticks` (macro called it CONTACT_TICKS) and +0x4D3 is `vehicle.on_ground_ticks`
 * (macro called it AIRBORNE_TICKS). Mapped each site to the DB member by OFFSET; the resulting logic is
 * self-consistent — airborne_ticks counts ticks since the last ground contact (advances while airborne,
 * reset on contact), on_ground_ticks advances while a mass point is in contact. So the local flag that
 * the old draft named `airborne` actually signals ground contact; renamed to `on_ground`. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/_object_definition.h"
#include "headers/physics_definition.h"
#include "headers/mass_point_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/blam_data_globals.h"

void compute_airborne_ticks(int vehicle_index, mass_point_datum *mass_points)
{
    char on_ground = 0;

    vehicle_datum *vehicle_obj =
        (vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum;

    /* object definition -> physics sub-tag index -> physics definition base */
    _object_definition *object_def = TAG_GET(_object_definition, vehicle_obj->definition_index);
    physics_definition *physics_def = TAG_GET(physics_definition, object_def->physics.index);

    /* airborne_ticks advances every tick, reset whenever a mass point contacts ground below */
    if (vehicle_obj->vehicle.airborne_ticks != 0xFF)
        ++vehicle_obj->vehicle.airborne_ticks;

    if (physics_def->mass_points.count > 0)  /* mass point count */
    {
        int i = 0;
        do
        {
            int flags = mass_points[i].flags;
            /* no DB enum found: runtime mass_point_datum.flags bits (searched mass_point/airborne/
             * contact/grounded in types_enum_values). bit1 = ground contact, bit4 = grounded contact. */
            if (flags & 2)
            {
                on_ground = 1;
                vehicle_obj->vehicle.airborne_ticks = 0;
                break;
            }
            if (flags & 0x10)
                vehicle_obj->vehicle.airborne_ticks = 0;
            i = (__int16)(i + 1);
        }
        while (i < physics_def->mass_points.count);
    }

    if (on_ground)
    {
        if (vehicle_obj->vehicle.on_ground_ticks != 0xFF)
            ++vehicle_obj->vehicle.on_ground_ticks;
    }
    else
    {
        vehicle_obj->vehicle.on_ground_ticks = 0;
    }
}
