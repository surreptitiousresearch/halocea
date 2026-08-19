/* _rasterizer_debug_model_vertices @0x836A6178 — debug visualisation of a skinned model part's
 * transformed vertices. Compiled out in this build: the body is empty (a stub that takes the object
 * index, the skinning palette and the geometry part but performs no work). Kept as a real translation
 * unit so the call site and symbol resolve. */

#include "headers/render_skinning.h"
#include "headers/gbxmodel_geometry_part.h"

void _rasterizer_debug_model_vertices(int target_object_index, const render_skinning *skinning,
        const gbxmodel_geometry_part *part)
{
    (void)target_object_index;
    (void)skinning;
    (void)part;
}
