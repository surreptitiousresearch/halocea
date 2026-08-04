/* rasterizer_dx9_shaders_vshader9_get 0x83723838 — return the D3D vertex shader at the given table slot, marking
 * it used and recording its filename as the last-fetched shader. Three debug-flag-gated maintenance passes run
 * first: vneed_clear zeroes the shader pointer slots, and vneed_dump_used / vneed_dump_unused print the filenames
 * of the (un)referenced shaders. The maintenance loops rely on the data-segment adjacency of hcex_last_vsh /
 * vshader_table_used / vneed_clear (as the original did) — preserved here as the raw pointer walks. */

#include "headers/rasterizer_dx9_shader_tables.h"
#include "headers/blam_data_globals.h"


extern void hcex_output_dbg(const char *format, ...);

D3DVertexShader *rasterizer_dx9_shaders_vshader9_get(unsigned int index)
{
    if ( vneed_clear )
    {
        /* zero the 65 pointer slots following hcex_last_vsh in the data segment */
        void **slot = (void **)&hcex_last_vsh;
        for ( int remaining = 65; remaining; --remaining )
            *++slot = nullptr;
    }

    if ( vneed_dump_used )
    {
        hcex_output_dbg("used:\n");
        /* recovered: parallel `p_filename += 2` walk over vsf_table -> indexed vsf_table[i].filename;
           original loop bound `used < &vneed_clear` is the data-segment end of vshader_table_used[65] */
        for ( int i = 0; i < 65; ++i )
            if ( vshader_table_used[i] )
                hcex_output_dbg("%s\n", vsf_table[i].filename);
    }

    if ( vneed_dump_unused )
    {
        hcex_output_dbg("used:\n");
        /* recovered: parallel `p_filename += 2` walk over vsf_table -> indexed vsf_table[i].filename */
        for ( int i = 0; i < 65; ++i )
            if ( !vshader_table_used[i] )
                hcex_output_dbg("%s\n", vsf_table[i].filename);
    }

    D3DVertexShader *result = vsf_table[index].shader;
    char *filename = vsf_table[index].filename;
    vshader_table_used[index] = 1;
    hcex_last_vsh = filename;
    return result;
}
