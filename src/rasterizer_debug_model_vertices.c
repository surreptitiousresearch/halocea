/* rasterizer_debug_model_vertices @0x836A6178 */
#include "headers/render_skinning.h"
#include "headers/gbxmodel_geometry_part.h"

extern void _rasterizer_debug_model_vertices(int target_object_index, const render_skinning *skinning,
                                             const gbxmodel_geometry_part *part);

void rasterizer_debug_model_vertices(int target_object_index, const render_skinning *skinning,
                                     const gbxmodel_geometry_part *part)
{
    /* blr no-op in binary — debug visualization stub */
}
