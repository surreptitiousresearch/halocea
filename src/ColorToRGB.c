/* ColorToRGB @ 0x837E3FA8 — pack an RGBA8 texel (S3TC_COLOR) into a 16-bit R5G6B5
 * value. Blue -> high 5 bits, green -> middle 6 bits, red -> low 5 bits. */

#include <stdint.h>
#include "headers/S3TC_COLOR.h"

void ColorToRGB(S3TC_COLOR *pcolor, uint16_t *prgb)
{
    *prgb = (uint16_t)((8 * ((32 * pcolor->rgba[2]) & 0xFF00 | pcolor->rgba[1])) & 0xFFE0
                               | (pcolor->rgba[0] >> 3));
}
