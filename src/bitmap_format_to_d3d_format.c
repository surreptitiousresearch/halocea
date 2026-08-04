#include "headers/d3d_render_boundary.h"

/* DEVIATION: decompiler names the lookup table 'table'; disasm confirms it is the same symbol. */
extern _D3DFORMAT table[];

_D3DFORMAT bitmap_format_to_d3d_format(__int16 format, unsigned __int16 flags)
{
    return table[format];
}
