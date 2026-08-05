/* area_of_effect_cause_damage @0x836B4CB8 — apply a damage tag's area-of-effect to everything in range.
 * Reads the damage (jpt!) definition's outer radius (doubled when the HCEX double-damage cheat is active),
 * collects every object within that sphere of the damage origin, damages each, then applies the same blast
 * to breakable surfaces.
 *
 * NOTE: the database prototype for objects_in_sphere undercounts its arguments. Per the disassembly the call
 * passes 8 arguments — including the damage tag-instance record and a maximum count of 64 — so a corrected
 * extern is declared here. */

#include <stdint.h>
#include "headers/damage_data.h"
#include "headers/damage_effect_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"


#include "headers/real_point3d.h"
extern int16_t objects_in_sphere(unsigned int class_flags, unsigned int type_flags, const location *location, const real_point3d *center, float radius, int *object_indices, int16_t maximum_count);
extern void area_of_effect_cause_damage_to_object(damage_data *damage_data, int object_index, uint8_t damage_next_object);
extern void breakable_surface_damage_area_of_effect(const damage_data *damage_data);

void area_of_effect_cause_damage(damage_data *damage_data, int unlucky_object_index)
{
    cache_file_tag_instance *definition = TAG_INSTANCE_ELEMENT(damage_data->definition_index);
    const damage_effect_definition *jolt = (const damage_effect_definition *)definition->base_address;
    float radius = jolt->cutoff_radius;
    int object_indices[72];
    int16_t count;

    if ( hcex_double_damage_radius )
        radius = jolt->cutoff_radius * 2.0f;

    /* attested 7-param: phantom `int definition` (float-radius GPR shadow slot r7) dropped */
    count = objects_in_sphere(0, 0, &damage_data->location, &damage_data->origin, radius,
                              object_indices, 64);
    if ( count > 0 )
    {
        int16_t i;
        for ( i = 0; i < count; ++i )
            area_of_effect_cause_damage_to_object(damage_data, object_indices[i], 0);
    }
    breakable_surface_damage_area_of_effect(damage_data);
}
