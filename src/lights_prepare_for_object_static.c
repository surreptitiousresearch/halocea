/* lights_prepare_for_object_static @0x836FA6A0 — compute an object's baked distant-lighting environment.
 *
 * It always samples the distant lighting at the object's center. For objects that are NOT dynamically lit
 * (object flag 0x4000 clear) it additionally samples the four horizontal corners of the bounding sphere
 * (+-radius/sqrt(2) in x and y at the center's height) and averages every successful sample together, so the
 * baked lighting represents the object's whole footprint rather than a single point. The averaged distant-light
 * directions and the shadow vector are renormalized. If no corner sample succeeded the last sampled block is
 * used as-is; a single successful sample is left unaveraged.
 *
 * The `flags` passed to lights_distant_lighting_at_point combine object flag 0x8000 (bit 0) with the object
 * definition's flags bit 4 (bit 2).
 *
 * DEVIATION: the decompiler rendered the sample-count reciprocal via a soft-float int->float bit construction
 * (a std/lfd reinterpret with a 0x82000000 high word); disasm confirms it is simply 1.0f / (float)sample_count
 * (fcfid/frsp/fdivs). */

#include <stdint.h>
#include <math.h>
#include <string.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/render_lighting.h"
#include "headers/real_point3d.h"
#include "headers/object_header_datum.h"
#include "headers/_object_definition.h"
#include "headers/object_flags.h"
#include "headers/object_definition_flags.h"
#include "headers/distant_lighting_flags.h"
#include "headers/blam_data_globals.h"

#define INV_SQRT2 0.70710677f

extern uint8_t lights_distant_lighting_at_point(int flags, const real_point3d *position, render_lighting *lighting);

static void normalize_vector(real_vector3d *v)
{
    float magnitude = sqrtf((v->n[0] * v->n[0]) + ((v->n[1] * v->n[1]) + (v->n[2] * v->n[2])));
    if ( fabsf(magnitude) >= 0.000099999997f )
    {
        float inverse = 1.0f / magnitude;
        v->n[0] = v->n[0] * inverse;
        v->n[1] = v->n[1] * inverse;
        v->n[2] = v->n[2] * inverse;
    }
}

void lights_prepare_for_object_static(int object_index, render_lighting *lighting)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    _object_definition *definition =
        TAG_GET(_object_definition, object->definition_index);

    int flags = (object->object.flags & (1u << _object_static_lighting_raycast_sideways_bit)) != 0;
    if ( (definition->flags & (1u << _object_artificially_bright_bit)) != 0 )
        flags |= (1u << _distant_lighting_brighten_bit);

    unsigned char center_ok = lights_distant_lighting_at_point(flags, &object->object.bounding_sphere_center,
                                                               lighting);

    if ( (object->object.flags & (1u << _object_static_lighting_recompute_bit)) != 0 )
        return;   /* dynamically lit objects only get the single center sample */

    int16_t sample_count;
    render_lighting sample;
    if ( center_ok )
    {
        sample_count = 1;
    }
    else
    {
        sample_count = 0;
        memset(lighting, 0, sizeof(render_lighting));
        lighting->distant_light_count = 2;
    }

    for ( int16_t corner = 0; corner < 4; corner++ )
    {
        float x_sign = (corner & 1) ? INV_SQRT2 : -INV_SQRT2;
        float y_sign = (corner & 2) ? INV_SQRT2 : -INV_SQRT2;
        float radius = object->object.bounding_sphere_radius;

        real_point3d position;
        position.n[0] = (radius * x_sign) + object->object.bounding_sphere_center.n[0];
        position.n[1] = (radius * y_sign) + object->object.bounding_sphere_center.n[1];
        position.n[2] = object->object.bounding_sphere_center.n[2];

        if ( lights_distant_lighting_at_point(flags, &position, &sample) )
        {
            lighting->ambient_color.n[0] += sample.ambient_color.n[0];
            lighting->ambient_color.n[1] += sample.ambient_color.n[1];
            lighting->ambient_color.n[2] += sample.ambient_color.n[2];

            lighting->reflection_tint_color.n[0] += sample.reflection_tint_color.n[0];
            lighting->reflection_tint_color.n[1] += sample.reflection_tint_color.n[1];
            lighting->reflection_tint_color.n[2] += sample.reflection_tint_color.n[2];
            lighting->reflection_tint_color.n[3] += sample.reflection_tint_color.n[3];

            for ( int d = 0; d < 2; d++ )
            {
                lighting->distant_lights[d].color.n[0] += sample.distant_lights[d].color.n[0];
                lighting->distant_lights[d].color.n[1] += sample.distant_lights[d].color.n[1];
                lighting->distant_lights[d].color.n[2] += sample.distant_lights[d].color.n[2];
                lighting->distant_lights[d].direction.n[0] += sample.distant_lights[d].direction.n[0];
                lighting->distant_lights[d].direction.n[1] += sample.distant_lights[d].direction.n[1];
                lighting->distant_lights[d].direction.n[2] += sample.distant_lights[d].direction.n[2];
            }

            ++sample_count;

            lighting->shadow_color.n[0] += sample.shadow_color.n[0];
            lighting->shadow_color.n[1] += sample.shadow_color.n[1];
            lighting->shadow_color.n[2] += sample.shadow_color.n[2];

            lighting->shadow_vector.n[0] += sample.shadow_vector.n[0];
            lighting->shadow_vector.n[1] += sample.shadow_vector.n[1];
            lighting->shadow_vector.n[2] += sample.shadow_vector.n[2];
        }
    }

    if ( sample_count <= 1 )
    {
        if ( sample_count == 0 )
            memcpy(lighting, &sample, sizeof(render_lighting));
        return;
    }

    float inverse = 1.0f / (float)sample_count;

    lighting->ambient_color.n[0] *= inverse;
    lighting->ambient_color.n[1] *= inverse;
    lighting->ambient_color.n[2] *= inverse;

    lighting->reflection_tint_color.n[0] *= inverse;
    lighting->reflection_tint_color.n[1] *= inverse;
    lighting->reflection_tint_color.n[2] *= inverse;
    lighting->reflection_tint_color.n[3] *= inverse;

    for ( int d = 0; d < 2; d++ )
    {
        lighting->distant_lights[d].color.n[0] *= inverse;
        lighting->distant_lights[d].color.n[1] *= inverse;
        lighting->distant_lights[d].color.n[2] *= inverse;
        lighting->distant_lights[d].direction.n[0] *= inverse;
        lighting->distant_lights[d].direction.n[1] *= inverse;
        lighting->distant_lights[d].direction.n[2] *= inverse;
        normalize_vector(&lighting->distant_lights[d].direction);
    }

    lighting->shadow_color.n[0] *= inverse;
    lighting->shadow_color.n[1] *= inverse;
    lighting->shadow_color.n[2] *= inverse;

    lighting->shadow_vector.n[0] *= inverse;
    lighting->shadow_vector.n[1] *= inverse;
    lighting->shadow_vector.n[2] *= inverse;
    normalize_vector(&lighting->shadow_vector);
}
