/* ClipExtrema @0x837E4368 — S3TC/DXT block-compression helper (neighbors Square3x3/Quantize/AllSame/
 * EncodeBlockRGBColorKey). Clips the color line segment [*plower, *pupper] against the axis-aligned box
 * [0, wtPrimary] in R/G/B space (channels 0-2; alpha is untouched): for each channel, if the segment
 * crosses 0 or crosses wtPrimary's value for that channel, the endpoint on the "inside" side is replaced
 * with the exact intersection point (recomputed across all three channels at once).
 *
 * DEVIATION: the raw decompile walks `plower` via byte-pointer arithmetic (`(FCOLOR*)((char*)v2+4)` each
 * iteration, then reads offset `pupper-plower` / `wtPrimary-plower` bytes past the current pointer) rather
 * than plain per-channel indexing. This is algebraically identical to indexing `plower->rgba[channel]`,
 * `pupper->rgba[channel]`, `wtPrimary[channel]` for channel = 0,1,2 — rewritten that way here since the
 * byte-offset form is only an artifact of how the compiler folded the two pointer subtractions into
 * loop-invariant constants. */

#include "headers/fcolor.h"
/* DEVIATION: this was a file-local `static FCOLOR wtPrimary;` claiming a distinct object at 0x8422EE00.
 * There is no symbol there (16 zero bytes, no name), so the clip threshold was always 0.0f; the code references the
 * canonical global at 0x84184D00 (lis wtPrimary@ha @0x837E436C). Declared as float[3] to match
 * src/data/wtPrimary.c and the other consumers (ColorToFcolor.c, FcolorToColor.c). */
extern float wtPrimary[3];


void ClipExtrema(FCOLOR *plower, FCOLOR *pupper)
{
    for ( int channel = 0; channel < 3; ++channel )
    {
        float lower_channel = plower->rgba[channel];
        float upper_channel = pupper->rgba[channel];

        if ( (lower_channel < 0.0f) != (upper_channel < 0.0f) )
        {
            float t = -(lower_channel / (upper_channel - lower_channel));
            FCOLOR *target;
            if ( lower_channel >= 0.0f )
            {
                t = t - 1.0f;
                target = pupper;
            }
            else
            {
                target = plower;
            }

            target->rgba[2] = (pupper->rgba[2] - plower->rgba[2]) * t + target->rgba[2];
            target->rgba[1] = (pupper->rgba[1] - plower->rgba[1]) * t + target->rgba[1];
            target->rgba[0] = (pupper->rgba[0] - plower->rgba[0]) * t + target->rgba[0];
        }

        float threshold = wtPrimary[channel];

        if ( (lower_channel > threshold) != (upper_channel > threshold) )
        {
            float t = (threshold - lower_channel) / (upper_channel - lower_channel);
            FCOLOR *target;
            if ( lower_channel <= threshold )
            {
                t = t - 1.0f;
                target = pupper;
            }
            else
            {
                target = plower;
            }

            target->rgba[2] = (pupper->rgba[2] - plower->rgba[2]) * t + target->rgba[2];
            target->rgba[1] = (pupper->rgba[1] - plower->rgba[1]) * t + target->rgba[1];
            target->rgba[0] = (pupper->rgba[0] - plower->rgba[0]) * t + target->rgba[0];
        }
    }
}
