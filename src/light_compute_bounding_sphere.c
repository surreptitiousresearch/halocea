/* light_compute_bounding_sphere @ 0x836F8310 — compute the world-space bounding sphere of a dynamic
 * light, used to decide which structure clusters it touches. The base radius is either the light's
 * current runtime radius (light float[21]) or, when `maximum` is set, the definition's maximum extent
 * (def[12]*def[4]); it is scaled by the specular falloff (def[36]) for specular/maximum non-ambient
 * lights. The sphere center depends on the light's cutoff angle (def[20]): a wide light (>= 90 deg) or
 * lens-flare-dominant light is treated as a point at the light origin; narrower cone lights push the
 * center forward along the light direction so the cone is enclosed. Light datum stride 124 bytes; light
 * definition reached via TAG_INSTANCE(light int[1]). */

#include "headers/data_array.h"
#include "headers/light_definition_flags.h"
#include "headers/light_datum.h"
#include "headers/point_light_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"

#include <stdint.h>

void light_compute_bounding_sphere(uint16_t light_index, uint8_t maximum,
    uint8_t specular, uint8_t lens_flare_only, real_point3d *bounding_sphere_center,
    float *bounding_sphere_radius)
{
    light_datum *light = DATA_ARRAY_ELEMENT(light_data, light_datum, light_index);
    point_light_definition *light_definition = TAG_GET(point_light_definition, light->definition_index);

    float radius = maximum
        ? light_definition->geometry.radius_modifier_upper_bound * light_definition->geometry.radius
        : light->radius;
    if ( (light_definition->flags & (1u << _light_no_specular_bit)) == 0 && (specular || maximum) )
        radius = light_definition->geometry.specular_radius_multiplier * radius;

    float cutoff_angle = light_definition->geometry.cutoff_angle;
    if ( lens_flare_only && light_definition->geometry.lens_flare_radius > radius )
    {
        /* attestation deviation: the old "light+0x04" raw read was wrong — disasm 836F83A4..836F83B8
         * copies offsets 0x30/0x34/0x38 off the datum base (r11 = light_data->data + 0x7C*index),
         * i.e. light->position. The stale +4 reading came from a decompiler base at light+0x2C. */
        *bounding_sphere_center = light->position;
        *bounding_sphere_radius = light_definition->geometry.lens_flare_radius;
    }
    else if ( cutoff_angle >= 1.5707964f )       /* >= 90 deg: omnidirectional, sphere at light origin */
    {
        /* attestation deviation: disasm 836F8474..836F8488 copies 0x30/0x34/0x38(r11) = light->position
         * (not light+4; see note above). */
        *bounding_sphere_center = light->position;
        *bounding_sphere_radius = radius;
    }
    else if ( cutoff_angle >= 0.78539819f )      /* 45..90 deg: wide cone */
    {
        *bounding_sphere_radius = light_definition->geometry.runtime_sine_cutoff_angle * radius;
        float forward_offset = light_definition->geometry.runtime_cosine_cutoff_angle * radius;
        bounding_sphere_center->n[0] = light->forward.n[0] * forward_offset + light->position.n[0];
        bounding_sphere_center->n[1] = light->forward.n[1] * forward_offset + light->position.n[1];
        bounding_sphere_center->n[2] = light->forward.n[2] * forward_offset + light->position.n[2];
    }
    else                                          /* < 45 deg: narrow cone */
    {
        float forward_offset = radius / light_definition->geometry.runtime_cosine_cutoff_angle;
        *bounding_sphere_radius = forward_offset;
        bounding_sphere_center->n[0] = light->forward.n[0] * forward_offset + light->position.n[0];
        bounding_sphere_center->n[1] = light->forward.n[1] * forward_offset + light->position.n[1];
        bounding_sphere_center->n[2] = light->forward.n[2] * forward_offset + light->position.n[2];
    }
}
