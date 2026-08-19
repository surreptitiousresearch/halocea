/* create_slipping_effects @0x83760338 — spawn material "slip" effects under a vehicle's mass points
 * that are sliding on the ground (e.g. tires skidding). For each contacting mass point (flag bit 1)
 * whose lateral slip speed exceeds 0.03, a material effect is created at the mass point's contact
 * position, oriented by a blend of the mass-point normal and the slip direction, with intensity
 * ramping from 0 at 0.03 m/s to 1 at ~0.25 m/s. The effect/material come from the vehicle definition
 * and the physics mass-point definition.
 *
 * DEVIATIONS:
 *  - Two params, not three (disasm 2026-07-31: only r3/r4 read, r5 never touched). The DB proto's 2nd
 *    param `powered_mass_point_datum *` is a mislabel: the runtime record is the 304-byte
 *    `mass_point_datum` (stride `mulli 0x130`), so the param is typed mass_point_datum* directly and
 *    the DB's phantom 3rd param is dropped to match the caller's 2-arg extern.
 *  - material_effect_new's is_player arg is 0 (disasm `li r10,0`); the decompiler reused the r9 value
 *    that actually feeds the effect_index selection (FPR-shadow: the float scale reserves r9). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/vehicle_definition.h"
#include "headers/physics_definition.h"
#include "headers/mass_point_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/location.h"
#include "headers/powered_mass_point_datum.h"
#include "headers/mass_point_datum.h"
#include "headers/mass_point_flags.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"
extern float __fsqrts(float);

extern void material_effect_new(int effects_definition_index, int16_t effect_index, int16_t material_index, const real_point3d *origin, const real_vector3d *normal, const location *location, float scale, uint8_t is_player);

void create_slipping_effects(int vehicle_index, mass_point_datum *mass_points)
{
    object_datum *vehicle_object_datum = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum;
    vehicle_definition *vehicle_def = TAG_GET(vehicle_definition, vehicle_object_datum->definition_index);
    physics_definition *physics_def = TAG_GET(physics_definition, vehicle_def->object.physics.index);

    int effects_definition_index = vehicle_def->vehicle.material_effects.index;
    int mass_point_count = physics_def->mass_points.count;
    if (effects_definition_index == -1 || mass_point_count <= 0)
        return;

    for (int i = 0; i < mass_point_count; i = (int16_t)(i + 1))
    {
        /* runtime mass-point record (see deviation note: base param is a DB mislabel) */
        mass_point_datum *mass_point = &mass_points[i];
        mass_point_definition *mass_point_def = &((mass_point_definition *)physics_def->mass_points.address)[i];

        /* runtime mass_point_datum.flags: only points touching the ground are slip candidates */
        if ((mass_point->flags & (1u << _point_on_ground_bit)) == 0)
            continue;

        float slip_x = mass_point->velocity_relative_to_ground.n[0];
        float slip_y = mass_point->velocity_relative_to_ground.n[1];
        float slip_z = mass_point->velocity_relative_to_ground.n[2];
        float slip_speed = __fsqrts(slip_z * slip_z + (slip_x * slip_x + slip_y * slip_y));
        if (slip_speed <= 0.029999999f)
            continue;

        float normal_x = mass_point->ground_plane.normal.n[0];
        float normal_y = mass_point->ground_plane.normal.n[1];
        float normal_z = mass_point->ground_plane.normal.n[2];
        float base_x = mass_point->position.n[0];
        float base_y = mass_point->position.n[1];
        float base_z = mass_point->position.n[2];

        float slip_scale = 0.86602539f / slip_speed; /* sqrt(3)/2 normalized by slip speed */
        float surface_offset = (mass_point->ground_depth - mass_point_def->radius) + 0.003f;

        real_vector3d effect_normal;
        effect_normal.n[0] = normal_x * 0.5f + slip_x * slip_scale;
        effect_normal.n[1] = normal_y * 0.5f + slip_scale * slip_y;
        effect_normal.n[2] = normal_z * 0.5f + slip_z * slip_scale;

        real_point3d effect_origin;
        effect_origin.n[0] = normal_x * surface_offset + base_x;
        effect_origin.n[1] = normal_y * surface_offset + base_y;
        effect_origin.n[2] = normal_z * surface_offset + base_z;

        float intensity = (slip_speed - 0.029999999f) * 4.5454545f;
        if (intensity >= 0.0f)
        {
            if (intensity > 1.0f)
                intensity = 1.0f;
        }
        else
        {
            intensity = 0.0f;
        }

        int mass_point_flags = mass_point_def->flags;
        material_effect_new(effects_definition_index,
                            (mass_point_flags & 1) ? 10 : 9,
                            mass_point->ground_material_type,
                            &effect_origin, &effect_normal,
                            &vehicle_object_datum->object.location,
                            intensity, 0);
    }
}
