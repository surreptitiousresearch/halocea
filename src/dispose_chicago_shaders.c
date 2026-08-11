/* dispose_chicago_shaders @0x836A1458 — release every compiled Chicago (multitexture) pixel shader in
 * chicago_shader_table and reset the table count, then release the decal pixel shader.
 *
 * DEVIATION: the decompiler models the table base with a -4 bias base and `[9]`/`+= 9` (dword) strides;
 * that is chicago_shader_table[i].shader (entry 36 bytes, shader at +32). The
 * decal teardown is a 5-iteration loop that releases the single decal_shader once (the pointer is reread
 * as null after the first pass) and nulls it; reproduced faithfully. */

#include "headers/chicago_shaders.h"
#include "headers/d3d_shader_boundary.h"
#include "headers/blam_data_globals.h"


void dispose_chicago_shaders(void)
{
    for (int i = 0; i < chicago_shader_table_size; ++i)
    {
        D3DResource_Release((D3DResource *)chicago_shader_table[i].shader);
        chicago_shader_table[i].shader = nullptr;
    }
    chicago_shader_table_size = 0;

    void *result = decal_shader;
    int remaining = 5;
    do
    {
        if (result)
            D3DResource_Release((D3DResource *)result);
        --remaining;
        decal_shader = nullptr;
        result = nullptr;
    }
    while (remaining);
}
