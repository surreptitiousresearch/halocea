/* _rasterizer_model_setupnodeparts @0x8378AD48 — upload a model's per-node skinning matrices to vertex
 * shader constant register 0x1D onward, one 12-float (3-vector4) block per node, gathered from the global
 * `vsh_constants__nodematrices` table (already-posed node matrices for the current frame) via the model's
 * local node table. Also records the node table/count as the "current" ones for later lookups.
 *
 * DEVIATION 1: the decompiler unrolled the node copy loop by 4 for register scheduling and split each
 * 12-float block's reads/writes across dozens of pseudo-registers with an off-by-one `char v53` stack
 * artifact ("local variable allocation failed"); reproduced here as the behaviorally identical single loop
 * copying 12 floats per node — no unrolling changes what is read or written.
 * DEVIATION 2: the incoming `node_matrices` parameter (the DB's own 3-arg prototype) is never read in this
 * compiled body — every node matrix comes from the `vsh_constants__nodematrices` global instead. Kept in
 * the signature (it matches callers using this exact prototype elsewhere) but unused, faithfully.
 * DEVIATION 3: `vsh_constants__nodematrices[12 * node]` reproduces the decompiler's
 * `4*node + 4*__ROL4__(node, 1)` index arithmetic — ROL-by-1 of an 8-bit node index is just node*2, so the
 * expression reduces to `12 * node` (each node = a 12-float / 3-vector4 vertex-shader-constant block).
 * DEVIATION 4: PendingMask0 for the final upload depends on `num_nodes` at runtime (unlike the fixed
 * `N << k` constants used by other rasterizer draw passes), so the decompiler's arithmetic-shift bit trick
 * (build a run of set bits via a signed shift of a single top bit, then reposition it) is kept verbatim
 * rather than simplified to a literal. */

#include <stdint.h>
#include "headers/real_matrix4x3.h"
#include "headers/d3d_boundary.h"
#include "headers/blam_data_globals.h"



void _rasterizer_model_setupnodeparts(int num_nodes, const uint8_t *node_table,
                                      real_matrix4x3 *node_matrices)
{
    g_CurrentLocalNodeTable     = node_table;
    g_CurrentLocalNodeTableSize = num_nodes;

    if (num_nodes >= 22)
        num_nodes = 22;

    float node_constants[264]; /* 22 nodes * 12 floats */
    for (int i = 0; i < num_nodes; ++i)
    {
        const float *src = &vsh_constants__nodematrices[12 * node_table[i]];
        float       *dst = &node_constants[12 * i];
        for (int c = 0; c < 12; ++c)
            dst[c] = src[c];
    }

    /* PendingMask0: a run of (shift_amount+1) set bits built via a signed right-shift of the top bit,
     * then repositioned 7 bits down — reproduces the disasm's extldi/srad/srdi sequence verbatim. */
    __int64      mask_seed    = (__int64)((unsigned __int64)1 << 63);
    unsigned int shift_amount = ((unsigned int)(3 * num_nodes + 28) >> 2) - 7;
    unsigned __int64 pending_mask = (unsigned __int64)(mask_seed >> shift_amount) >> 7;

    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0x1D, node_constants, 3 * num_nodes, pending_mask);
}
