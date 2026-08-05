/* FcolorToColor @ 0x837E3F30 — convert a floating-point FCOLOR back to a packed
 * RGBA8 texel (S3TC_COLOR): un-weight by wtPrimary and rescale [0,1] -> [0,255].
 * Alpha is not written. */

#include <stdint.h>
#include "headers/S3TC_COLOR.h"
#include "headers/fcolor.h"

extern float wtPrimary[3];

void FcolorToColor(FCOLOR *pfcolor, S3TC_COLOR *pcolor)
{
    pcolor->rgba[0] = (uint8_t)(int)((pfcolor->rgba[0] / wtPrimary[0]) * 255.0f);
    pcolor->rgba[1] = (uint8_t)(int)((pfcolor->rgba[1] / wtPrimary[1]) * 255.0f);
    /* Deviation: decompiler emitted a spurious ">>24" on this final channel only
     * (register-width artifact); the three channels are symmetric, so it is dropped. */
    pcolor->rgba[2] = (uint8_t)(int)((pfcolor->rgba[2] / wtPrimary[2]) * 255.0f);
}
