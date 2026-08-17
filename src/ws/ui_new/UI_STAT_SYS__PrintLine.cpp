/* ?PrintLine@UI_STAT_SYS@ui_new@@AAAXAAVUI_STAT_RENDER@2@PB_WABUUI_STAT_PARAMS@2@HUm2dV@@@Z @0x82CEFF58 */
#include "../../headers/ws/ui_new/UI_STAT_SYS.h"

// 0x82CEFFA8 — draw one already-split (no embedded '\n') line of `lineLength` characters,
// anchored via AdjustStringPos. Each glyph is looked up in the font's sorted letter table by
// character code (falling back to '.' for an unmapped glyph), optionally drawn with a 1px
// drop-shadow box first, then the glyph itself tinted by params.color (packed 0xAARRGGBB from
// its already-0..255-range float channels). The pen advances by each glyph's own width (plus the
// shared per-character interval) between characters.
void ui_new::UI_STAT_SYS::PrintLine(UI_STAT_RENDER &renderer, const wchar_t *str,
                                     const UI_STAT_PARAMS &params, int lineLength, m2dV pos)
{
    m2dV anchor = AdjustStringPos(pos, lineLength, (char)params.alignment, params.scale, params.interval);
    if (lineLength <= 0)
        return;

    unsigned int packedColor =
        ((unsigned int)(unsigned char)(int)params.color.a << 24) |
        ((unsigned int)(unsigned char)(int)params.color.r << 16) |
        ((unsigned int)(unsigned char)(int)params.color.g << 8) |
        ((unsigned int)(unsigned char)(int)params.color.b);

    float penX = anchor.x;
    for (int i = 0; i < lineLength; ++i)
    {
        int ch = str[i];
        UI_STAT_FONT::CmpFindLetter cmp;
        int idx = font.m_Letters.FindSorted<UI_STAT_FONT::CmpFindLetter, int>(ch, cmp);
        if (idx < 0)
        {
            int dot = '.';
            idx = font.m_Letters.FindSorted<UI_STAT_FONT::CmpFindLetter, int>(dot, cmp);
        }
        const fntLETTER &letter = font.m_Letters[idx];

        if (str[i] != L' ')
        {
            float l = letter.poly.l * params.scale + penX;
            float t = letter.poly.t * params.scale + anchor.y;
            float r = letter.poly.r * params.scale + penX;
            float b = letter.poly.b * params.scale + anchor.y;

            if (params.hasShadow)
            {
                m3dRECT shadowRect = {l + 1.0f, t + 1.0f, r + 1.0f, b + 1.0f};
                renderer.AddBox(0xFF000000u, shadowRect, letter.uv);
            }

            m3dRECT glyphRect = {l, t, r, b};
            renderer.AddBox(packedColor, glyphRect, letter.uv);
        }

        penX += (letter.poly.r - letter.poly.l + params.interval) * params.scale;
    }
}
