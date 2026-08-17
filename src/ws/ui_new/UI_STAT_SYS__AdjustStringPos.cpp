/* ?AdjustStringPos@UI_STAT_SYS@ui_new@@AAA?AUm2dV@@U3@HHMM@Z @0x82CED340 */
#include "../../headers/ws/ui_new/UI_STAT_SYS.h"

// 0x82CED340 — offset a line's anchor `pos` by its measured extent according to `alignment`:
// horizontal bits (alignment & 7): 2 = shift left by the line's full total advance width
// (right-aligns the run so it ends at `pos`), 4 = shift left by half the advance width (centers
// the whole run around `pos`), else unchanged. Vertical bits (alignment & 0x70): 0x20 = shift up
// by the font's descent (bbox.b), 0x40 = shift up by half the font's line height (centers
// vertically), else shift up by the font's ascent (bbox.t).
// DEVIATION: `pos` is passed BY VALUE (a plain m2dV packed into one 64-bit GPR, split into
// x/y halves via stack spill), not a pointer — the decompiler mistyped it as `m2dV *` and
// rendered every read of its x/y halves as `*(float *)&this` / `*(float *)&pos` (raw
// pointer-bit-pattern reinterpretation), which is meaningless taken literally. Reconstructed here
// from the disassembly's actual float loads (lfs f9, arg_20(r1) / arg_20+4(r1), the stack slot
// `pos` was spilled to) rather than the corrupted pseudocode.
m2dV ui_new::UI_STAT_SYS::AdjustStringPos(m2dV pos, int lineLength, char alignment, float scale, float interval)
{
    m2dV result = pos;
    float lineWidth = (font.bbox.r - font.bbox.l + interval) * (float)lineLength * scale;

    if ((alignment & 7) == 2)
        result.x = pos.x - lineWidth;
    else if ((alignment & 7) == 4)
        result.x = pos.x - lineWidth * 0.5f;

    if ((alignment & 0x70) == 0x20)
        result.y = pos.y - font.bbox.b * scale;
    else if ((alignment & 0x70) == 0x40)
        result.y = pos.y - (font.bbox.b + font.bbox.t) * 0.5f * scale;
    else
        result.y = pos.y - font.bbox.t * scale;

    return result;
}
