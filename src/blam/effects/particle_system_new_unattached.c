/* particle_system_new_unattached @0x8373BD38 — spawn a free-standing particle system datum at a world point
 * with an initial velocity and color. Disabled (returns -1) when particle-system rendering is off. Stamps
 * position/velocity/color/scale into the datum, samples the static lighting at the point, then runs the
 * system's one-time initialization; on failure the datum is released and -1 returned.
 *
 * Particle system datum (344 bytes), accessed by field offset: flags @0x04, definition_index @0x08,
 * group/parent @0x0C (-1), position @0x20, velocity @0x2C, color rgb @0x38 + alpha @0x44, scale @0x14,
 * sampled light color @0x18. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/particle_system_datum.h"
#include "headers/particle_system_flags.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_argb_color.h"
#include "headers/real_rgb_color.h"
#include "headers/blam_data_globals.h"

extern int datum_new(data_array *data);
extern void datum_delete(data_array *data, int index);
extern void light_particle(const real_point3d *point, real_rgb_color *light_color, real_rgb_color *diffuse_color, uint8_t block);
extern uint8_t particle_system_initialize(int new_system_index);

int particle_system_new_unattached(int definition_index, const real_point3d *position,
        const real_vector3d *velocity, const real_argb_color *color, float scale)
{
    if ( !render_particle_systems_enabled )
        return -1;

    int index = datum_new(particle_systems);
    if ( index == -1 )
        return index;

    particle_system_datum *record = DATA_ARRAY_ELEMENT(particle_systems, particle_system_datum, index);
    record->object_index = -1;
    record->definition_index = definition_index;
    record->position = *position;
    record->velocity = *velocity;
    record->color.n[0] = color->n[0];
    record->color.n[1] = color->n[1];
    record->color.n[2] = color->n[2];
    record->scale = scale;
    record->color.n[3] = color->n[3];
    record->flags |= 1u << _particle_system_active_bit;

    real_rgb_color diffuse_discard;
    light_particle(&record->position, &record->lighting, &diffuse_discard, 0);

    if ( particle_system_initialize(index) )
        return index;

    datum_delete(particle_systems, index);
    return -1;
}
