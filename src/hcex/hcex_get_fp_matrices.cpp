/* hcex_get_fp_matrices @0x83682FC8 — look up the cached first-person node-matrix array for a model on a
 * given camera slot. Two model slots per camera are tracked (hcex_fp_model1/2); if modelId matches the
 * model registered for that camera, the corresponding matrix array is returned, else null.
 *
 * Each matrix array is 0xD00 bytes (per disasm `mulli r10, r4, 0xD00`) = 64 * sizeof(real_matrix4x3),
 * i.e. up to 64 node transforms; the returned pointer aliases the first matrix. cam_idx is bounds-checked
 * to [0,1]. */

#include "../headers/real_matrix4x3.h"

extern int hcex_fp_model1[2];
extern int hcex_fp_model2[2];
extern real_matrix4x3 hcex_fp_model_matrices1[2][64];
extern real_matrix4x3 hcex_fp_model_matrices2[2][64];

extern "C" real_matrix4x3 *hcex_get_fp_matrices(int modelId, unsigned int cam_idx)
{
    if ( cam_idx > 1 )
        return 0;
    if ( modelId == hcex_fp_model1[cam_idx] )
        return hcex_fp_model_matrices1[cam_idx];
    if ( modelId == hcex_fp_model2[cam_idx] )
        return hcex_fp_model_matrices2[cam_idx];
    return 0;
}
