/* rasterizer_dx9_shaders_write_to_binary @ 0x836A2020
   Dev tooling: compiles every pixel-shader effect and writes it to the binary
   blob "D:\maps\fx.bin", then resets the chicago/decal fixed-function shaders.

   Deviation: the decompiler renders the set_chicago_shader stage-config
   arguments as a tangle of __int128 register bit-rotations of zero — they all
   evaluate to null/zero (the stage configs are zero-initialized). Reproduced
   here as the null/zero calls they actually are. */

#include <stdint.h>
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/chicago_shaders.h"
#include "headers/win32_boundary.h"

extern uint8_t rasterizer_dx9_shaders_write_effect(unsigned int shaderIndex, void *fileHandle);

/* attest: uint8_t return — the succeeded flag is kept byte-normalized (clrlwi r29 @
   0x836A20FC) and returned via mr r3,r29 (or li r3,0 on CreateFileA failure); the
   sole caller ignores the value. */
extern void rasterizer_dx9_setup_effect_macros(void);
uint8_t rasterizer_dx9_shaders_write_to_binary(void)
{
    /* DEVIATION: verbatim inlined body of rasterizer_dx9_setup_effect_macros@0x836A0CE0 (zero-xref out-of-line twin); collapsed to call, no params to fold */
    rasterizer_dx9_setup_effect_macros();

    void *file = CreateFileA("D:\\maps\\fx.bin", 0x40000000u, 0, 0, 2u, 0x8000000u, 0);
    if ( file == WIN32_INVALID_HANDLE )
        return 0;

    uint8_t succeeded = 1;
    for ( int index = 0; index < 123 && succeeded; ++index )
    {
        if ( !rasterizer_dx9_shaders_write_effect(index, file) )
            succeeded = 0;
    }

    shader_bin_handle = file;
    /* the decompiler's __int128 register arithmetic resolves to zeroed stage
       configs and zero scalar args for both passes */
    shader_stage_cfg zero_stage = { 0, 0, 0 };
    set_chicago_shader(0, zero_stage, zero_stage, zero_stage, 0, 0);
    set_chicago_shader(1, zero_stage, zero_stage, zero_stage, 0, 0);
    set_decal_shader(0);
    shader_bin_handle = (void *)-1;
    CloseHandle(file);
    return succeeded;
}
