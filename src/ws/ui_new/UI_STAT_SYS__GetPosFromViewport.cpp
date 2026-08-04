#include "../../headers/ws/ui_new/UI_STAT_SYS.h"

// 0x82CED99C — anchor position for a multi-line string within `viewport`, honoring the same
// alignment bit layout as AdjustStringPos: horizontal (alignment & 7): 2 = viewport.r, 4 =
// viewport's horizontal center, else viewport.l. Vertical (alignment & 0x70): 0x20 = bottom-
// anchored (accounts for the full block height, i.e. all lines), 0x40 = vertically centered
// (half the block height), else viewport.t (top-anchored, unadjusted).
m2dV ui_new::UI_STAT_SYS::GetPosFromViewport(const wchar_t *str, char alignment, float scale)
{
    m2dV result;

    if ((alignment & 7) == 2)
        result.x = viewport.r;
    else if ((alignment & 7) == 4)
        result.x = (viewport.r + viewport.l) * 0.5f;
    else
        result.x = viewport.l;

    float lineHeight = font.bbox.b - font.bbox.t;
    if ((alignment & 0x70) == 0x20)
        result.y = viewport.b - (float)GetLinesNumber(str) * lineHeight * scale;
    else if ((alignment & 0x70) == 0x40)
        result.y = (viewport.b + viewport.t) * 0.5f - (float)GetLinesNumber(str) * lineHeight * scale * 0.5f;
    else
        result.y = viewport.t;

    return result;
}
