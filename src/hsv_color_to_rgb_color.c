/* hsv_color_to_rgb_color @0x8377D2C0 — convert a fixed-point HSV color to fixed-point RGB. Inverse of
 * rgb_color_to_hsv_color.c. Hue is stored as n[0]/65536 (a full turn), saturation and value as n[1]/65535 and
 * n[2]/65535; the result channels are written back as round(channel * 65535). This is the textbook
 * sextant HSV->RGB conversion.
 *
 * DEVIATION: Hex-Rays reported "local variable allocation has failed" and rendered the whole body as an
 * unreadable tangle of __int64 register puns, HIDWORD/LODWORD splits, __SPAIR64__ constants (the integer
 * sextant index N mis-shown as a float), and reads of `*(__int64 *)(&v2 - 1)`. Reconstructed from disasm
 * 0x8377D2C8-0x8377D484: the (V,t,p)/(q,V,p)/... sextant table and the value/sat/hue scales below match the
 * disassembly exactly. The i > 5 branch is unreachable for a normalized hue and the binary simply reads
 * uninitialized stack there. */

#include <stdint.h>
#include "headers/hsv_color.h"
#include "headers/rgb_color.h"

rgb_color * hsv_color_to_rgb_color(const hsv_color *hsv, rgb_color *rgb)
{
    float value = (float)hsv->n[2] * 0.000015259022f;      /* / 65535 */
    float saturation = (float)hsv->n[1] * 0.000015259022f; /* / 65535 */
    float hue_sextant = (((float)hsv->n[0] * 0.000015258789f) * 6.0f); /* (hue / 65536) * 6 */

    float red, green, blue;

    if ( saturation == 0.0f )
    {
        red = value;
        green = value;
        blue = value;
    }
    else
    {
        int sextant = (int)hue_sextant;
        if ( (float)sextant > hue_sextant )
            --sextant;

        if ( sextant > 5 )
        {
            /* unreachable for hue < 1.0; the binary leaves the channels as uninitialized stack */
        }
        else
        {
            float fraction = hue_sextant - (float)sextant;
            float p = (((float)1.0 - saturation) * value);
            float q = (((float)1.0 - (fraction * saturation)) * value);
            float t = (((float)1.0 - (((float)1.0 - fraction) * saturation)) * value);

            switch ( sextant )
            {
                case 0:  red = value; green = t;     blue = p;     break;
                case 1:  red = q;     green = value; blue = p;     break;
                case 2:  red = p;     green = value; blue = t;     break;
                case 3:  red = p;     green = q;     blue = value; break;
                case 4:  red = t;     green = p;     blue = value; break;
                default: red = value; green = p;     blue = q;     break; /* case 5 */
            }
        }
    }

    rgb->__s1.red   = (uint16_t)(red * 65535.0f);   /* union view: red/green/blue live in __s1 */
    rgb->__s1.green = (uint16_t)(green * 65535.0f);
    rgb->__s1.blue  = (uint16_t)(blue * 65535.0f);
    return rgb;
}
