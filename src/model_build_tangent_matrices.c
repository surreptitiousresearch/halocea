/* model_build_tangent_matrices @0x83799590 — disasm_range(0x83799590,0x837995EC) confirms this is genuinely
 * a no-op double-count loop with zero `bl` calls and no memory writes; reproduced faithfully as-is. */

#include "headers/model.h"
#include "headers/model_geometry.h"

void model_build_tangent_matrices(model *model)
{
    int geometry_count = model->geometries.count;

    for ( int geometry_index = 0; geometry_index < geometry_count; geometry_index++ )
    {
        int part_count = ((const model_geometry *)model->geometries.address)[geometry_index].parts.count;

        for ( int part_index = 0; part_index < part_count; part_index++ )
            ;
    }
}
