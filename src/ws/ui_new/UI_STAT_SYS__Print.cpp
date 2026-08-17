/* ?Print@UI_STAT_SYS@ui_new@@AAAXAAVUI_STAT_RENDER@2@Um2dV@@PB_WABUUI_STAT_PARAMS@2@@Z @0x82CF01B8 */
#include "../../headers/ws/ui_new/UI_STAT_SYS.h"

// 0x82CF01D8 — split `str` on '\n' and PrintLine each segment, advancing `pos.y` by one font
// line-height between lines. No-op for a null/empty string, or while the viewport is degenerate
// (near-zero width/height). DEVIATION: the real parameter order (renderer, pos, str, params) was
// recovered from the disassembly's register-setup comments at this function's call sites and its
// own prologue (`mr r27,r4 # renderer`), not from the decompiler's own signature for THIS
// function, which mislabeled `pos` as the 2nd (renderer) slot and dropped `renderer` entirely
// (the same by-value-m2dV register-packing artifact seen in AdjustStringPos/PrintLine).
void ui_new::UI_STAT_SYS::Print(UI_STAT_RENDER &renderer, m2dV pos, const wchar_t *str,
                                 const UI_STAT_PARAMS &params)
{
    if (!str)
        return;
    if (!(viewport.r - viewport.l >= 0.000001f && viewport.b - viewport.t >= 0.000001f))
        return;
    if (!*str)
        return;

    const wchar_t *p = str;
    while (*p)
    {
        const wchar_t *lineStart = p;
        int lineLength = 0;
        while (*p && *p != L'\n')
        {
            ++p;
            ++lineLength;
        }
        if (lineLength)
            PrintLine(renderer, lineStart, params, lineLength, pos);

        if (*p)
        {
            ++p;
            pos.y += (font.bbox.b - font.bbox.t) * params.scale;
        }
    }
}
