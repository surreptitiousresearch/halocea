/* rasterizer_dx9_shaders_initialize @ 0x836A23D0
   Builds the pixel-shader effect set. Sets the shared effect macros (Xbox,
   ps_3_0 target), creates the effect pool, then loads the effects (precompiled
   binary preferred, ASCII fallback). On success it allocates and resolves the
   named effect-parameter handles for every shader that needs them; on failure
   it raises an error dialog. Returns whether the effects loaded.

   Two transcription helpers keep the ~45 identical parameter bindings readable:
     SHADER_ALLOC(idx, bytes, line) -> shader_table[idx].constants = dlMalloc(...)
     BIND(idx, slot, name)          -> constants[slot] = GetParameterByName(name)
   The dlMalloc source-line arguments are preserved verbatim from the binary. */

#include <stdint.h>
#include "headers/rasterizer_dx9_shader_table.h"

extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern int DisplayErrorDialog(int Error, int ErrorURL, int ForceExit);
extern uint8_t rasterizer_dx9_shaders_write_to_binary(void);
extern uint8_t rasterizer_dx9_shaders_load_from_binary(void);
extern uint8_t rasterizer_dx9_shaders_load_from_ascii(void);

#define SHADERS_SRC "D:\\Projects\\code\\HCEX\\sources\\rasterizer\\dx9\\rasterizer_dx9_shaders.c"
#define SHADER_ALLOC(idx, bytes, line) \
    shader_table[idx].constants = (unsigned int *)dlMalloc((bytes), SHADERS_SRC, (line))
/* C boundary wrapper for opaque COM object (was effect->GetParameterByName) */
#define BIND(idx, slot, name) \
    shader_table[idx].constants[slot] = \
        ID3DXEffect_GetParameterByName(shader_table[idx].effect, 0, (name))

extern void rasterizer_dx9_setup_effect_macros(void);
uint8_t rasterizer_dx9_shaders_initialize(void)
{
    /* DEVIATION: verbatim inlined body of rasterizer_dx9_setup_effect_macros@0x836A0CE0 (zero-xref out-of-line twin); collapsed to call, no params to fold */
    rasterizer_dx9_setup_effect_macros();

    int succeeded = D3DXCreateEffectPool(&global_effect_pool) >= 0;

    if ( dump_psh_to_binary )
        rasterizer_dx9_shaders_write_to_binary();

    if ( !rasterizer_dx9_shaders_load_from_binary() )
    {
        if ( !succeeded || !rasterizer_dx9_shaders_load_from_ascii() )
            succeeded = 0;
    }

    if ( !succeeded )
    {
        DisplayErrorDialog(105, 126, 1);
        return succeeded;
    }

    /* environment shaders: primary change color, fog correction, self illum */
    for ( int idx = 116; idx <= 121; ++idx )
    {
        SHADER_ALLOC(idx, 0x14u, 0x367);
        BIND(idx, 0, "c_primary_change_color");
        BIND(idx, 1, "c_fog_color_correction_0");
        BIND(idx, 2, "c_fog_color_correction_E");
        BIND(idx, 3, "c_fog_color_correction_1");
        BIND(idx, 4, "c_self_illumination_color");
    }

    /* glass-style view-dependent tint shaders */
    for ( int idx = 32; idx <= 34; ++idx )
    {
        SHADER_ALLOC(idx, 0xCu, 0x376);
        BIND(idx, 0, "c_eye_forward");
        BIND(idx, 1, "c_view_perpendicular_color");
        BIND(idx, 2, "c_view_parallel_color");
    }
    for ( int idx = 37; idx <= 39; ++idx )
    {
        SHADER_ALLOC(idx, 0xCu, 0x383);
        BIND(idx, 0, "c_eye_forward");
        BIND(idx, 1, "c_view_perpendicular_color");
        BIND(idx, 2, "c_view_parallel_color");
    }

    SHADER_ALLOC(106, 0x10u, 0x38F);
    BIND(106, 0, "c_eye_forward");
    BIND(106, 1, "c_view_perpendicular_color");
    BIND(106, 2, "c_view_parallel_color");
    BIND(106, 3, "c_group_intensity");

    SHADER_ALLOC(107, 0xCu, 0x39C);
    BIND(107, 0, "c_eye_forward");
    BIND(107, 1, "c_view_perpendicular_color");
    BIND(107, 2, "c_view_parallel_color");

    SHADER_ALLOC(108, 0xCu, 0x3A8);
    BIND(108, 0, "c_eye_forward");
    BIND(108, 1, "c_view_perpendicular_color");
    BIND(108, 2, "c_view_parallel_color");

    /* plasma material shader */
    SHADER_ALLOC(0, 0x18u, 0x3B4);
    BIND(0, 0, "c_material_color");
    BIND(0, 1, "c_plasma_animation");
    BIND(0, 2, "c_primary_color");
    BIND(0, 3, "c_secondary_color");
    BIND(0, 4, "c_plasma_on_color");
    BIND(0, 5, "c_plasma_off_color");

    /* material-color-only shaders 1..3 (re-allocated below for shader 1) */
    for ( int idx = 1; idx <= 3; ++idx )
    {
        SHADER_ALLOC(idx, 4u, 0x3C0);
        BIND(idx, 0, "c_material_color");
    }

    SHADER_ALLOC(114, 8u, 0x3CB);
    BIND(114, 0, "c_desaturation_tint");
    BIND(114, 1, "c_light_enhancement");

    SHADER_ALLOC(1, 0xCu, 0x3D3);
    BIND(1, 0, "c0");
    BIND(1, 1, "c1");
    BIND(1, 2, "c2");

    SHADER_ALLOC(47, 4u, 0x3DA);
    BIND(47, 0, "c0");

    SHADER_ALLOC(76, 4u, 0x3DF);
    BIND(76, 0, "c0");

    SHADER_ALLOC(36, 4u, 0x3E4);
    BIND(36, 0, "c1");

    SHADER_ALLOC(4, 8u, 0x3E9);
    BIND(4, 0, "c0");
    BIND(4, 1, "c1");

    SHADER_ALLOC(112, 0x10u, 0x3EF);
    BIND(112, 0, "c0");
    BIND(112, 1, "c1");
    BIND(112, 2, "c2");
    BIND(112, 3, "c3");

    /* specular view-dependent shaders */
    for ( int idx = 40; idx <= 43; ++idx )
    {
        SHADER_ALLOC(idx, 0x10u, 0x3FB);
        BIND(idx, 0, "c_specular_brightness");
        BIND(idx, 1, "c_view_perpendicular_color");
        BIND(idx, 2, "c_view_parallel_color");
        BIND(idx, 3, "c_multiplier");
    }

    return succeeded;
}
