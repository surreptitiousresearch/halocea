/* rasterizer_dx9_shaders_vshader9_initialize @ 0x83723BA0
   Loads the vertex-shader set. Prefers the precompiled binary blob; if that
   isn't present, compiles each named shader from its .vso source. Optionally
   first dumps the shaders to a binary blob when dump_vsh_to_binary is set.

   Deviation resolved 2026-08-11: the decompiler renders the leading zero loop as
   65 aliased float writes from console_color.n[3]. The binary's own base register
   names the table (`lis/addi r29, vsf_table` @0x83723BAC), so the aliasing was
   never ambiguous — it is vsf_table[i].shader, and is spelled that way below. */

#include <stdint.h>
#include "headers/rasterizer_dx9_shader_tables.h"
#include "headers/real_argb_color.h"
#include "headers/blam_data_globals.h"

extern uint8_t rasterizer_dx9_shaders_vshader9_write_to_binary(void);
extern uint8_t rasterizer_dx9_shaders_vshader9_load_from_binary(void);
extern uint8_t rasterizer_dx9_shaders_vshader9_load_vso_file(int index);

uint8_t rasterizer_dx9_shaders_vshader9_initialize(void)
{
    /* 0x83723BBC-0x83723BCC: `addi r11, r29, -8` then 65x `stwu r28(=0), 8(r11)` off the vsf_table
     * base — 65 stride-8 stores at offset 0 of each entry, i.e. vsf_table[i].shader. The old
     * `float* = &console_color.n[3]; p += 2` walk hit the same bytes only because vsf_table sits at
     * console_color+0x14 in this 32-bit image; the hard-coded 8-byte stride is wrong on x64. */
    for ( int i = 0; i < 65; ++i )
        vsf_table[i].shader = 0;


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
