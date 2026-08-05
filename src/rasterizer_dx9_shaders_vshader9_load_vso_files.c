/* rasterizer_dx9_shaders_vshader9_load_vso_files @0x83723720 — load every vso file named in vsf_table,
 * returning true only if every named entry loaded successfully.
 *
 * DEVIATION: the decompiler rendered the loop bound as `&keyboard_auxbutton_mapping[2]`, an unrelated
 * adjacent global; disasm_range(0x83723720,0x837237A0) shows the true bound is `vsf_table + 0x20C`
 * (r29 = vsf_table, r31 starts at r29+4 stepping by 8 bytes/entry) — i.e. the end of vsf_table itself
 * (65 entries, matching rasterizer_dx9_shader_tables.h's `vertex_shader_function vsf_table[65]`). Rewritten
 * as a plain indexed loop over vsf_table. */

#include <stdint.h>
#include "headers/rasterizer_dx9_shader_tables.h"

extern uint8_t rasterizer_dx9_shaders_vshader9_load_vso_file(int index);

uint8_t rasterizer_dx9_shaders_vshader9_load_vso_files(void)
{
    uint8_t result = 1;

    for ( int i = 0; i < 65 && result; ++i )
    {
        if ( vsf_table[i].filename )
            result = rasterizer_dx9_shaders_vshader9_load_vso_file(i);
    }

    return result;
}
