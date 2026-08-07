/* rasterizer_set_model_lighting @0x8369CEC0 — stage a model's resolved lighting into vertex-shader constants
 * (2 point lights via rasterizer_set_model_lighting_point_light, 2 distant lights inline, ambient) and upload
 * them starting at register 15. Debug override: if model_lighting_ambient <= 0, upload a pure grey-ambient
 * block instead (memset to 0, ambient = model_lighting_ambient on all 3 channels). Recovered the real upload
 * call args from disasm — the decompiler showed Vector4fCount/PendingMask0 as 0x1E000000u/a stray pointer;
 * real values are count=11, mask=(unsigned __int64)15<<57. FAITHFUL QUIRK: the per-distant-light validity
 * check ORs the count comparison with `&lighting->distant_lights[i] == NULL`, which (lighting always being a
 * valid non-null pointer) is realistically always false — reproduced as-is from the disasm's literal
 * address-vs-zero compare rather than "fixed" to a single count check. */

#include <stdint.h>
#include "headers/render_lighting.h"
#include "headers/rasterizer_model_lighting_constants.h"
#include "headers/d3d_render_boundary.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/blam_data_globals.h"


extern void rasterizer_set_model_lighting_point_light(int light_index, int16_t light_num, rasterizer_model_lighting_constants *lighting_constants);
extern void *memset(void *dest, int value, unsigned int size);
extern void D3DDevice_SetVertexShaderConstantFN(D3DDevice *device, unsigned int StartRegister,
                                                const float *pConstantData, unsigned int Vector4fCount,
                                                uint64_t PendingMask0);

void rasterizer_set_model_lighting(const render_lighting *lighting)
{
    rasterizer_model_lighting_constants constants;

    if ( rasterizer_debug_options.model_lighting_ambient <= 0.0f )
    {
        memset(&constants, 0, sizeof(constants));
        constants.ambient.n[0] = rasterizer_debug_options.model_lighting_ambient;
        constants.ambient.n[1] = rasterizer_debug_options.model_lighting_ambient;
        constants.ambient.n[2] = rasterizer_debug_options.model_lighting_ambient;
    }
    else
    {
        for ( int16_t light_num = 0; light_num < 2; light_num++ )
        {
            int light_index = light_num < lighting->point_light_count ? lighting->point_light_indices[light_num] : -1;
            rasterizer_set_model_lighting_point_light(light_index, light_num, &constants);
        }

        for ( int16_t light_index = 0; light_index < 2; light_index++ )
        {
            if ( light_index >= lighting->distant_light_count
              || (uintptr_t)((const char *)&lighting->distant_lights[light_index]) == 0 )
            {
                constants.distant_lights[light_index].forward.n[0] = 0.0f;
                constants.distant_lights[light_index].forward.n[1] = 0.0f;
                constants.distant_lights[light_index].forward.n[2] = 0.0f;
                constants.distant_lights[light_index].pad0 = 0.0f;
                constants.distant_lights[light_index].color.n[0] = 0.0f;
                constants.distant_lights[light_index].color.n[1] = 0.0f;
                constants.distant_lights[light_index].color.n[2] = 0.0f;
                constants.distant_lights[light_index].pad1 = 0.0f;
            }
            else
            {
                constants.distant_lights[light_index].forward = lighting->distant_lights[light_index].direction;
                constants.distant_lights[light_index].color = lighting->distant_lights[light_index].color;
            }
        }

        constants.ambient = lighting->ambient_color;
    }

    D3DDevice_SetVertexShaderConstantFN(global_d3d_device, 0xF, (const float *)&constants, 11, (uint64_t)15 << 57);
}
