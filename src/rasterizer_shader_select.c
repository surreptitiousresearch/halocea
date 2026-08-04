/* rasterizer_shader_select @ 0x836A1E18
   Returns the shader_table entry for `shader_index`, marking it used. Honors a
   few debug toggles: pneed_clear resets the used-table; pneed_dump_used /
   pneed_dump_unused print the used/unused effect filenames. */

#include <stdint.h>
#include "headers/rasterizer_dx9_shader_table.h"
#include "headers/blam_data_globals.h"

extern void hcex_output_dbg(const char *format, ...);

rasterizer_dx9_shader *rasterizer_shader_select(int16_t shader_index)
{
    if ( pneed_clear )
    {
        for ( int i = 0; i < 123; ++i )
            pshader_table_used[i] = 0;
    }
    if ( pneed_dump_used )
    {
        hcex_output_dbg("used:\n");
        for ( int i = 0; i < 123; ++i )
            if ( pshader_table_used[i] )
                hcex_output_dbg("%s\n", shader_table[i].filename);
    }
    if ( pneed_dump_unused )
    {
        hcex_output_dbg("used:\n"); /* BINARY-FAITHFUL: original prints "used:" header for the unused list too (same string @0x836A1EC8) */
        for ( int i = 0; i < 123; ++i )
            if ( !pshader_table_used[i] )
                hcex_output_dbg("%s\n", shader_table[i].filename);
    }

    pshader_table_used[shader_index] = 1;
    hcex_last_fx = shader_table[shader_index].filename;
    return &shader_table[shader_index];
}
