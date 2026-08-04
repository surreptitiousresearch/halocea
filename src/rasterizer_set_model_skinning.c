/* rasterizer_set_model_skinning @0x8369CBF0 — populate the global `vsh_constants__nodematrices` staging
 * table from a model's posed node-matrix palette (scaling the rotation/basis rows but not the translation
 * row), then optionally upload it to vertex shader constant register 0x1D onward. This is the producer for
 * the table `_rasterizer_model_setupnodeparts` later gathers per-part node blocks from.
 *
 * DEVIATION: PendingMask0 for the upload again depends on the runtime node count (same arithmetic-shift
 * bit trick as `_rasterizer_model_setupnodeparts` / `_rasterizer_detail_objects_draw`); confirmed identical
 * via disasm (`extldi r7,r9,64,63; srad; srdi ,,7`), only the base register naming differs. */

#include <stdint.h>
#include "headers/render_skinning.h"
#include "headers/rasterizer_globals.h"
#include "headers/d3d_boundary.h"
#include "headers/blam_data_globals.h"


void rasterizer_set_model_skinning(const render_skinning *skinning, uint8_t uploadconstants)
{
    __int16 node_count = skinning->node_matrix_count;
    if (rasterizer_globals.maximum_nodes_per_model < node_count)
        node_count = rasterizer_globals.maximum_nodes_per_model;

    for (int i = 0; i < node_count; ++i)
    {
        const real_matrix4x3 *src = &skinning->node_matrices[i];
        float                 scale = src->scale;
        float                *dst = &vsh_constants__nodematrices[12 * i];

        dst[0]  = src->n[0][0] * scale;
        dst[1]  = src->n[1][0] * scale;
        dst[2]  = src->n[2][0] * scale;
        dst[3]  = src->n[3][0];
        dst[4]  = src->n[0][1] * scale;
        dst[5]  = src->n[1][1] * scale;
        dst[6]  = src->n[2][1] * scale;
        dst[7]  = src->n[3][1];
        dst[8]  = src->n[0][2] * scale;
        dst[9]  = src->n[1][2] * scale;
        dst[10] = src->n[2][2] * scale;
        dst[11] = src->n[3][2];
    }

    if (!uploadconstants)
        return;

    __int64      mask_seed    = (__int64)((unsigned __int64)1 << 63);
    unsigned int shift_amount = ((unsigned int)(3 * node_count + 28) >> 2) - 7;
    unsigned __int64 pending_mask = (unsigned __int64)(mask_seed >> shift_amount) >> 7;

    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0x1D, vsh_constants__nodematrices,
                                        3 * node_count, pending_mask);
}
