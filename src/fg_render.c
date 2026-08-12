/* fg_render @0x83790868 — draw the framerate graph and its text labels. When either the graph or the info
 * readout is enabled, sets up the dynamic screen-geometry vertex declaration/shader, uploads a 2D ortho
 * projection (built from the current viewport size) as vertex-shader constants, pushes the fixed-function /
 * render state for untextured point-sprite-enabled line drawing, then — only when the graph itself is enabled —
 * draws the 512-vertex plot line strip and the 5-vertex border, and renders the "FPS" title plus the max/average
 * value labels.
 *
 * DEVIATIONS: the decompiler mismodeled the projection-constant setup as an __int128 / g_PFP[208] dereference;
 * the disassembly shows a plain ortho matrix derived from the viewport width/height (1/w, 1/h scaling). The
 * viewport int->float and the max/average float->int (for "%d") are fcfid/fctiwz artifacts restored as casts. */

#include <stdint.h>
#include "headers/fg_graph.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/real_argb_color.h"
#include "headers/d3d_render_boundary.h"
#include "headers/d3d_shader_boundary.h"
#include "headers/_D3DTEXTURESTAGESTATETYPE.h"
#include "headers/_D3DPRIMITIVETYPE.h"
#include "headers/rasterizer_vertex_shader_declaration_index.h"
#include "headers/rasterizer_vertex_shader_index.h"
#include "headers/_D3DCULL.h"
#include "headers/blam_data_globals.h"

typedef struct D3DPixelShader D3DPixelShader; /* boundary type — not reversed */


#include "headers/point2d.h"
extern D3DVertexDeclaration *rasterizer_dx9_shaders_vdecl9_get(unsigned int index);
extern D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index);
extern void D3DDevice_SetVertexDeclaration(D3DDevice *device, D3DVertexDeclaration *declaration);
extern void D3DDevice_SetVertexShader(D3DDevice *device, D3DVertexShader *shader);
extern void D3DDevice_SetVertexShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
                                                const float *pConstantData, unsigned int Vector4fCount,
                                                uint64_t PendingMask0);
extern void D3DDevice_SetPixelShader(D3DDevice *device, D3DPixelShader *shader);
extern void rasterizer_set_stencil_mode(int16_t stencil_mode);
extern void D3DDevice_SetRenderState_CullMode(D3DDevice *device, unsigned int mode);
extern void D3DDevice_SetRenderState_ColorWriteEnable(D3DDevice *device, unsigned int mask);
extern void D3DDevice_SetRenderState_AlphaBlendEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_AlphaTestEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_ZEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_ZWriteEnable(D3DDevice *device, unsigned int enable);
extern void D3DDevice_SetRenderState_PointSpriteEnable(D3DDevice *device, unsigned int enable);
extern void SetTextureStageStateSmart(unsigned int stage, _D3DTEXTURESTAGESTATETYPE State, unsigned int Value);
extern void D3DDevice_DrawVerticesUP(D3DDevice *device, unsigned int primitive_type, unsigned int vertex_count,
                                     const void *vertex_data, unsigned int vertex_stride);

extern void interface_set_bitmap_text_draw_mode(int16_t interface_font_index, int16_t style, int16_t justification, unsigned int flags, int16_t color_table_index, int16_t color_index);
extern void draw_string_set_tab_stops(const int16_t *tab_stops, int16_t count);
extern void draw_string_set_color(const real_argb_color *color);
extern void rasterizer_draw_string(const rectangle2d *bounds, const rectangle2d *clip, point2d *cursor_reference, int16_t height_adjust, const char *string);
extern int sprintf_0(char *string, const char *format, ...);

void fg_render(uint8_t render_graph, uint8_t render_infos)
{
    real_argb_color white;
    white.n[0] = 1.0f;
    white.n[1] = 1.0f;
    white.n[2] = 1.0f;
    white.n[3] = 1.0f;

    if (!render_graph && !render_infos)
        return;

    D3DDevice_SetVertexDeclaration(global_d3d_device, rasterizer_dx9_shaders_vdecl9_get(_vsdecl_screen));
    D3DDevice_SetVertexShader(global_d3d_device, rasterizer_dx9_shaders_vshader9_get(_vs_screen));

    rectangle2d *viewport = &global_window_parameters.camera.viewport_bounds;
    int viewport_width = viewport->n[3] - viewport->n[1];
    int viewport_height = viewport->n[2] - viewport->n[0];

    /* 2D screen-space ortho projection as a vertex-shader constant block (5 float4 rows) */
    float projection[20];
    projection[0]  = 2.0f / (float)viewport_width;
    projection[1]  = 0.0f;
    projection[2]  = 0.0f;
    projection[3]  = -1.0f - 1.0f / (float)viewport_width;
    projection[4]  = 0.0f;
    projection[5]  = -2.0f / (float)viewport_height;
    projection[6]  = 0.0f;
    projection[7]  = 1.0f / (float)viewport_height + 1.0f;
    projection[8]  = 0.0f;
    projection[9]  = 0.0f;
    projection[10] = 0.0f;
    projection[11] = 0.5f;
    projection[12] = 0.0f;
    projection[13] = 0.0f;
    projection[14] = 0.0f;
    projection[15] = 1.0f;
    projection[16] = 1.0f;
    projection[17] = 1.0f;
    projection[18] = 0.0f;
    projection[19] = 1.0f;

    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xD, projection, 5, (uint64_t)3 << 59);
    D3DDevice_SetPixelShader(global_d3d_device, nullptr);
    rasterizer_set_stencil_mode(0);
    D3DDevice_SetRenderState_CullMode(global_d3d_device, D3DCULL_NONE);
    D3DDevice_SetRenderState_ColorWriteEnable(global_d3d_device, 0xF);
    D3DDevice_SetRenderState_AlphaBlendEnable(global_d3d_device, 0);
    D3DDevice_SetRenderState_AlphaTestEnable(global_d3d_device, 0);
    D3DDevice_SetRenderState_ZEnable(global_d3d_device, 0);
    D3DDevice_SetRenderState_ZWriteEnable(global_d3d_device, 0);
    SetTextureStageStateSmart(0, D3DTSS_COLOROP, 3);  /* COLOROP = SELECTARG1 */
    SetTextureStageStateSmart(0, D3DTSS_COLORARG2, 0);  /* COLORARG2 */
    SetTextureStageStateSmart(0, D3DTSS_ALPHAOP, 3);  /* ALPHAOP = SELECTARG1 */
    SetTextureStageStateSmart(0, D3DTSS_ALPHAARG2, 0);  /* ALPHAARG2 */
    SetTextureStageStateSmart(1, D3DTSS_COLOROP, 1);  /* stage 1 COLOROP = DISABLE */
    SetTextureStageStateSmart(1, D3DTSS_ALPHAOP, 1);  /* stage 1 ALPHAOP = DISABLE */
    D3DDevice_SetTexture(global_d3d_device, 0, nullptr, 0x80000000);
    D3DDevice_SetRenderState_PointSpriteEnable(global_d3d_device, 1);

    if (render_graph)
    {
        D3DDevice_DrawVerticesUP(global_d3d_device, D3DPT_LINESTRIP, 0x200, fg_graphs[0].vertices, 0x18);
        D3DDevice_DrawVerticesUP(global_d3d_device, D3DPT_LINESTRIP, 5, fg_graphs[0].border, 0x18);
    }

    interface_set_bitmap_text_draw_mode(1, -1, 0, 0, 5, 0);
    draw_string_set_tab_stops(nullptr, 0);
    draw_string_set_color(&white);

    if (render_graph)
    {
        char text[520];
        rasterizer_draw_string(&fg_graphs[0].label_extents, nullptr, nullptr, 0, fg_graphs[0].label);
        sprintf_0(text, "%d", (int)fg_graphs[0].max);
        rasterizer_draw_string(&fg_graphs[0].label_max_extents, nullptr, nullptr, 0, text);
        sprintf_0(text, "%d", (int)fg_graphs[0].average);
        rasterizer_draw_string(&fg_graphs[0].label_avg_extents, nullptr, nullptr, 0, text);
    }
}
