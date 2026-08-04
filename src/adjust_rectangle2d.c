/* adjust_rectangle2d @0x83799F48 — position `source`'s width/height inside `bounds`, written to
 * `destination` (also returned): mode 0 centers on both axes; mode 1 centers horizontally but offsets
 * vertically by only 1/3 of the leftover space from the top (not centered); any other mode copies `source`
 * unmodified. All arithmetic is on the plain (width, height) extents — the result keeps source's size,
 * just repositioned within bounds's origin.
 *
 * DEVIATION: disasm_range(0x83799F48,0x8379A0D8) shows the decompiler's `v24`
 * (`*(_DWORD*)source->n - source->n[0]`) is dead: it only feeds the display of the (unreachable-as-shown)
 * default branch, which disasm proves is a plain `*destination = *source` with no arithmetic at all. The
 * quantity actually used as "source height" in the mode 0/1 branches is a clean `source->y1 - source->y0`
 * computed earlier and reused — restored here as `source_height`. Mode 0's `(v>>1) + (v<0 && odd)` idiom is
 * exactly C's truncating `/` on each operand separately (`bounds_height/2 - source_height/2`, NOT
 * `(bounds_height-source_height)/2` — they differ when the two operands have opposite parity/sign, e.g.
 * bounds_height=1, source_height=-1 gives 0 split vs 1 combined), so mode 0 and mode 1's divisions are
 * written with genuinely different associativity to match. */

#include <stdint.h>
#include "headers/rectangle2d.h"

void adjust_rectangle2d(const rectangle2d *bounds, const rectangle2d *source, rectangle2d *destination, int16_t mode)
{
    __int16 bounds_width = bounds->x1 - bounds->x0;
    __int16 bounds_height = bounds->y1 - bounds->y0;
    __int16 source_width = source->x1 - source->x0;
    __int16 source_height = source->y1 - source->y0;

    if ( mode == 0 )
    {
        __int16 delta_x = bounds_width / 2 - source_width / 2;
        __int16 delta_y = bounds_height / 2 - source_height / 2;

        destination->x0 = bounds->x0 + delta_x;
        destination->x1 = bounds->x0 + delta_x + source_width;
        destination->y0 = bounds->y0 + delta_y;
        destination->y1 = bounds->y0 + delta_y + source_height;
    }
    else if ( mode == 1 )
    {
        __int16 delta_x = (bounds_width - source_width) / 2;
        __int16 delta_y = (bounds_height - source_height) / 3;

        destination->x0 = bounds->x0 + delta_x;
        destination->x1 = bounds->x0 + delta_x + source_width;
        destination->y0 = bounds->y0 + delta_y;
        destination->y1 = bounds->y0 + delta_y + source_height;
    }
    else
    {
        *destination = *source;
    }

    return;
}
