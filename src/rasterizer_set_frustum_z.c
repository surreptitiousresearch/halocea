/* rasterizer_set_frustum_z @ 0x8369D050
   Uploads the camera transform to the vertex shader constant registers. Unless
   called with the sentinel (-2,-2) — meaning "leave z as-is" — it first patches
   the frustum's projection z range. It then composes projection * world_to_view
   into a 4x4 (constant reg 0), appends the camera position (w=2) and forward
   (w=0.5), and finally uploads the upper rows of view_to_world (constant reg
   0x1B). The fourth arg to SetVertexShaderConstantFN is the Vector4fCount and
   the fifth a 64-bit PendingMask0 built with sldi (disasm 8369D15C/64 and
   8369D1B8/C0). */

#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/rasterizer_engine_globals.h"
#include "headers/d3d_shader_boundary.h"

extern void render_camera_hack_frustum_z(render_frustum *frustum, float z_near, float z_far);

void rasterizer_set_frustum_z(float z_near, float z_far)
{
    render_frustum *frustum = &global_window_parameters.frustum;

    if ( z_near != -2.0f || z_far != -2.0f )
        render_camera_hack_frustum_z(frustum, z_near, z_far);

    float combined[26];
    int last_row_base = 0;
    for ( int i = 0; i < 4; i = (short)(i + 1) )
    {
        for ( int j = 0; j < 4; j = (short)(j + 1) )
        {
            const float *view_row = frustum->world_to_view.n[j];
            combined[i * 4 + j] =
                  frustum->projection_matrix[0][i] * view_row[0]
                + frustum->projection_matrix[1][i] * view_row[1]
                + frustum->projection_matrix[2][i] * view_row[2];
        }
        last_row_base = 4 * i;
        combined[last_row_base + 3] = frustum->projection_matrix[3][i] + combined[last_row_base + 3];
    }

    combined[16] = global_window_parameters.camera.position.n[0];
    combined[17] = global_window_parameters.camera.position.n[1];
    combined[18] = global_window_parameters.camera.position.n[2];
    combined[19] = 2.0f;
    combined[20] = global_window_parameters.camera.forward.n[0];
    combined[21] = global_window_parameters.camera.forward.n[1];
    combined[22] = global_window_parameters.camera.forward.n[2];
    combined[23] = 0.5f;
    /* attest fix: disasm 8369D15C li r6,6 (Vector4fCount) + 8369D164 sldi r7,3,62
       (PendingMask0=0xC000000000000000) — decompiler had folded the mask into arg 4
       and threaded the loop index into arg 5 */
    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0, combined, 6, 0xC000000000000000ull);

    float view_to_world_rows[8];
    view_to_world_rows[3] = 1.0f;
    view_to_world_rows[7] = 3.0f;
    view_to_world_rows[2] = frustum->view_to_world.n[0][2];
    view_to_world_rows[4] = frustum->view_to_world.n[1][0];
    view_to_world_rows[5] = frustum->view_to_world.n[1][1];
    view_to_world_rows[6] = frustum->view_to_world.n[1][2];
    view_to_world_rows[0] = frustum->view_to_world.n[0][0];
    view_to_world_rows[1] = frustum->view_to_world.n[0][1];
    /* attest fix: disasm 8369D1B8 li r6,2 (Vector4fCount) + 8369D1C0 sldi r7,3,56
       (PendingMask0=0x0300000000000000) */
    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0x1B, view_to_world_rows, 2, 0x0300000000000000ull);
}
