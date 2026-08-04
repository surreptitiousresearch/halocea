/* ColorToFcolor @ 0x837E3EB0 — convert a packed RGBA8 texel (S3TC_COLOR) to a
 * floating-point FCOLOR, applying the per-channel wtPrimary weights and normalising
 * from [0,255] to [0,1] (factor 1/255 = 0.0039215689). Alpha is not written. */

#include "headers/S3TC_COLOR.h"
#include "headers/fcolor.h"

extern float wtPrimary[3];

void ColorToFcolor(S3TC_COLOR *pcolor, FCOLOR *pfcolor)
{
    pfcolor->rgba[0] = (float)pcolor->rgba[0] * wtPrimary[0] * (1.0f / 255.0f);
    pfcolor->rgba[1] = (float)pcolor->rgba[1] * wtPrimary[1] * (1.0f / 255.0f);
    pfcolor->rgba[2] = (float)pcolor->rgba[2] * wtPrimary[2] * (1.0f / 255.0f);
}
