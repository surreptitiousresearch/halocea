#include <stdint.h>
#include "headers/d3d_render_boundary.h"

/* DEVIATION: decompiler names the lookup table 'table'; disasm confirms it is the same symbol. */
extern _D3DFORMAT table[];

_D3DFORMAT bitmap_format_to_d3d_format(int16_t format, uint16_t flags)
{
    return table[format];
}
