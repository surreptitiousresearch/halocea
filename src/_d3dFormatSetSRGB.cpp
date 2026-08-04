// _d3dFormatSetSRGB @0x827BA0C8 — set (or clear) the per-channel sRGB gamma field of a packed
// Xenos texture format word. The gamma field is 6 bits (bits 9..14, three 2-bit per-RGB-channel
// gamma selectors): enable => all channels = 3 (sRGB), disable => 0 (linear).
#include "headers/_D3DFORMAT.h"

void _d3dFormatSetSRGB(_D3DFORMAT *format, bool enable)
{
    int gamma = enable ? 3 : 0; // per-channel gamma selector (R = G = B)
    unsigned int field = (unsigned int)(((4 * ((4 * gamma) | gamma)) | gamma) << 9);
    unsigned int *raw = (unsigned int *)format;
    *raw = field | (*raw & 0xFFFF81FF); // clear+set bits 9..14
}
