// ws-engine ai08 (a8_debug.cpp): aiDEBUG::InterpolateColor @0x83193EF8 (protected).
// Linearly interpolate the packed 0x00RRGGBB colour `colorFrom`->`colorTo` across `maxColors`
// steps, returning the colour at step `idx` (clamped to the endpoints outside [1, maxColors-1]).
// NOTE: the DB decompile lost its clamp assignments ("local variable allocation failed"); the
// _m3dClamp results are captured here per the evident intent (side-effect-only clamps would be a
// no-op). The per-channel lerp factor is idx/maxColors.
#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/m3d/m3d_boundary.h"   // _m3dClamp(a,b,value)

unsigned long aiDEBUG::InterpolateColor(unsigned int colorFrom, unsigned int colorTo,
                                        int maxColors, int idx)
{
    if (idx < 1)
        return colorFrom;
    if (idx >= maxColors - 1)
        return colorTo;

    float factor = static_cast<float>(idx) / static_cast<float>(maxColors);

    int rFrom = (colorFrom >> 16) & 0xFF, gFrom = (colorFrom >> 8) & 0xFF, bFrom = colorFrom & 0xFF;
    int rTo   = (colorTo   >> 16) & 0xFF, gTo   = (colorTo   >> 8) & 0xFF, bTo   = colorTo   & 0xFF;

    float r = static_cast<float>(rTo - rFrom) * factor + static_cast<float>(rFrom);
    float g = static_cast<float>(gTo - gFrom) * factor + static_cast<float>(gFrom);
    float b = static_cast<float>(bTo - bFrom) * factor + static_cast<float>(bFrom);

    r = _m3dClamp(0.0f, 255.0f, r);
    g = _m3dClamp(0.0f, 255.0f, g);
    b = _m3dClamp(0.0f, 255.0f, b);

    return (((static_cast<unsigned long>(static_cast<int>(r)) << 8) |
             static_cast<unsigned long>(static_cast<int>(g))) << 8) |
           static_cast<unsigned long>(static_cast<int>(b));
}
