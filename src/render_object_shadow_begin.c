/* render_object_shadow_begin 0x836E61C0 — begin rendering an object's projected shadow. Builds the shadow
 * projection matrix from the object's origin, a normalized vector perpendicular to the light's shadow vector,
 * and the shadow vector itself; fades the shadow color toward "no shadow" (white) as the level-of-detail factor
 * drops (and further for bipeds/vehicles running an active-camo style fade), then opens the environment shadow
 * pass.
 *
 * DEVIATION: the decompiler passed the object type (v17, read as a float*) as rasterizer_environment_shadow_
 * begin's 5th argument; the disassembly shows the real 5th argument is &data->shadow_bounding_radius (r7, sitting
 * past the float radius arg's GPR shadow slot). The object type is only used in the biped/vehicle fade test. */

#include <stdint.h>
#include "headers/object_render_data.h"
#include "headers/object_datum.h"
#include "headers/unit_datum.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_rgb_color.h"
#include "headers/ppc_intrinsics.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


#include "headers/real_matrix4x3.h"
extern real_vector3d *perpendicular3d(const real_vector3d *a, real_vector3d *result);
extern void matrix4x3_from_point_and_vectors(real_matrix4x3 *matrix, const real_point3d *point, const real_vector3d *forward, const real_vector3d *up);
extern unsigned __int8 rasterizer_environment_shadow_begin(int object_index, const real_matrix4x3 *shadow_matrix,
                                                           const real_rgb_color *light_color,
                                                           float object_bounding_radius,
                                                           float *shadow_volume_bounding_radius);

uint8_t render_object_shadow_begin(object_render_data *data, float lod)
{
    int object_index = (unsigned __int16)data->object_index;
    object_datum *object_data = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum);

    real_point3d object_origin;
    object_origin.n[0] = object_data->object.bounding_sphere_center.n[0];
    object_origin.n[1] = object_data->object.bounding_sphere_center.n[1];
    object_origin.n[2] = object_data->object.bounding_sphere_center.n[2];
    float object_bounding_radius = object_data->object.bounding_sphere_radius;

    real_vector3d shadow_right;
    perpendicular3d(&data->lighting->shadow_vector, &shadow_right);
    float magnitude = __fsqrts(shadow_right.n[2] * shadow_right.n[2]
                             + (shadow_right.n[0] * shadow_right.n[0] + shadow_right.n[1] * shadow_right.n[1]));
    if ( __fabs(magnitude) >= 0.000099999997 )
    {
        float inverse = 1.0f / magnitude;
        shadow_right.n[0] = inverse * shadow_right.n[0];
        shadow_right.n[1] = inverse * shadow_right.n[1];
        shadow_right.n[2] = inverse * shadow_right.n[2];
    }
    matrix4x3_from_point_and_vectors(&data->shadow_matrix, &object_origin, &shadow_right,
                                     &data->lighting->shadow_vector);

    real_rgb_color shadow_color;
    shadow_color.n[0] = data->lighting->shadow_color.n[0];
    shadow_color.n[1] = data->lighting->shadow_color.n[1];
    shadow_color.n[2] = data->lighting->shadow_color.n[2];

    int object_type = object_data->object.type;
    if ( ((1 << object_type) & object_mask_unit) != 0 && ((unit_datum *)object_data)->unit.active_camouflage > 0.0 )
        lod = ((1.0f - ((unit_datum *)object_data)->unit.active_camouflage) * lod);

    shadow_color.n[0] = shadow_color.n[0] * lod + (1.0f - lod);
    shadow_color.n[1] = shadow_color.n[1] * lod + (1.0f - lod);
    shadow_color.n[2] = shadow_color.n[2] * lod + (1.0f - lod);

    return rasterizer_environment_shadow_begin(data->object_index, &data->shadow_matrix, &shadow_color,
                                               object_bounding_radius, &data->shadow_bounding_radius);
}
