/* object_get_level_of_detail_pixels @0x836E60B0 — compute the on-screen size (in pixels) of an object's
 * bounding sphere, which the renderer uses to pick a level of detail.
 *
 * If a cinematic is in progress and the object is flagged to always render at full detail during cinematics
 * (object flags +0x10 bit 0x400000), the function short-circuits to FLT_MAX so the object always gets the
 * highest LOD. Otherwise it takes the object's bounding sphere (center at object data float[40..42], radius at
 * float[43]), scales the radius by the rasterizer model-quality level (0 -> x0.25, 1 -> x0.5, >=2 -> full), and
 * asks the frustum how many pixels that sphere's diameter projects to. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/render_globals.h"
#include "headers/real_point3d.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"

#include "headers/render_frustum.h"
extern uint8_t cinematic_in_progress(void);
extern float render_frustum_sphere_diameter_in_pixels(const render_frustum *frustum, const real_point3d *point, float radius);

float object_get_level_of_detail_pixels(int object_index)
{
    object_datum *object_data = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    if ( cinematic_in_progress() && (object_data->object.flags & (1u << _object_movie_star_bit)) != 0 )
        return 3.4028235e38f;

    real_point3d center;
    center.n[0] = object_data->object.bounding_sphere_center.n[0];
    center.n[1] = object_data->object.bounding_sphere_center.n[1];
    center.n[2] = object_data->object.bounding_sphere_center.n[2];

    float radius = object_data->object.bounding_sphere_radius;
    if ( rasterizer_debug_options.rasterizer_model_quality_level == 1 )
        radius = object_data->object.bounding_sphere_radius * 0.5f;
    else if ( rasterizer_debug_options.rasterizer_model_quality_level == 0 )
        radius = object_data->object.bounding_sphere_radius * 0.25f;

    return render_frustum_sphere_diameter_in_pixels(&render.frustum, &center, radius);
}
