/* render_object_shadows @0x836E7590 — render the environment shadow pass for all rendered objects. When
 * shadows are enabled, brackets a process_rendered_objects call (with object_index = -1 to mean "all
 * objects", shadow on, no planar fog) between rasterizer_environment_shadows_begin/end.
 *
 * Typed void: the helpers have void(void) prototypes, so the decompiler's int return / trailing
 * `return result` (uninitialized r3) on the shadows-disabled path is an artifact. shadow_matrix is left
 * uninitialized, matching the original. */

#include "headers/object_render_data.h"
#include "headers/blam_data_globals.h"

extern void rasterizer_environment_shadows_begin(void);
extern void rasterizer_environment_shadows_end(void);
extern void process_rendered_objects(object_render_data *data);

void render_object_shadows(void)
{
    if (!render_shadows)
        return;

    rasterizer_environment_shadows_begin();

    object_render_data render_data;
    render_data.object_index = -1;
    render_data.shadow = 1;
    render_data.lighting = nullptr;
    render_data.shadow_bounding_radius = 0.0f;
    render_data.no_planar_fog = 1;
    render_data.rasterizer_bitmap_cookie = -1;
    process_rendered_objects(&render_data);

    rasterizer_environment_shadows_end();
}
