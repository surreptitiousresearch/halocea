/* rasterizer_dx9_shaders_vshader9_initialize @ 0x83723BA0
   Loads the vertex-shader set. Prefers the precompiled binary blob; if that
   isn't present, compiles each named shader from its .vso source. Optionally
   first dumps the shaders to a binary blob when dump_vsh_to_binary is set.

   Deviation: the leading zero loop is rendered by the decompiler as 65 aliased
   float writes starting from console_color.n[3] (stride 2 floats). That walks
   the memory of the adjacent vsf_table region; it is reproduced literally here
   rather than guessed at, since the exact field aliasing is ambiguous. */

#include <stdint.h>
#include "headers/rasterizer_dx9_shader_tables.h"
#include "headers/real_argb_color.h"
#include "headers/blam_data_globals.h"

extern uint8_t rasterizer_dx9_shaders_vshader9_write_to_binary(void);
extern uint8_t rasterizer_dx9_shaders_vshader9_load_from_binary(void);
extern uint8_t rasterizer_dx9_shaders_vshader9_load_vso_file(int index);

uint8_t rasterizer_dx9_shaders_vshader9_initialize(void)
{
    /* clear 65 stride-8 slots in the table region adjacent to console_color */
    float *p = &console_color.n[3];
    for ( int count = 65; count; --count )
    {
        p += 2;
        *p = 0.0f;
    }

    if ( dump_vsh_to_binary )
        rasterizer_dx9_shaders_vshader9_write_to_binary();

    if ( rasterizer_dx9_shaders_vshader9_load_from_binary() )
        return 1;

    int succeeded = 1;
    for ( int index = 0; index < 65 && succeeded; ++index )
    {
        if ( vsf_table[index].filename )
        {
            if ( !rasterizer_dx9_shaders_vshader9_load_vso_file(index) )
                succeeded = 0;
        }
    }
    return succeeded;
}
