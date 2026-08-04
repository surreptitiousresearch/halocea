/* rgb_color_to_hsv_color @0x8377D108 — the fixed-point twin of real_rgb_color_to_real_hsv_color.c: convert
 * a 16-bit-per-channel RGB color to 16-bit-per-channel HSV. Each channel is normalised to [0,1] by 1/65535,
 * the standard max/min HSV math is applied, then hue/saturation/value are re-quantised to 16 bits (hue by
 * *65536 so a full turn wraps the range; saturation and value by *65535).
 *
 * DEVIATION: the decompiler failed local-variable allocation and rendered the channel loads as __int64
 * HIDWORD/LODWORD register puns and the destination pointer as an __int64; reconstructed from the identical
 * algorithm in real_rgb_color_to_real_hsv_color.c plus the observed output scaling/offsets (sat at n[1],
 * hue at n[0], value at n[2]). */

#include "headers/rgb_color.h"
#include "headers/hsv_color.h"

hsv_color * rgb_color_to_hsv_color(const rgb_color *rgb, hsv_color *hsv)
{
    float red   = rgb->__s1.red   * 0.000015259022f; /* 1/65535 */
    float green = rgb->__s1.green * 0.000015259022f;
    float blue  = rgb->__s1.blue  * 0.000015259022f;

    float value = (green <= blue) ? blue : green;
    if (red > value)
        value = red;

    float minimum = (green <= blue) ? green : blue;
    if (red <= minimum)
        minimum = red;

    float delta = value - minimum;
    float saturation = (value == 0.0f) ? 0.0f : delta / value;
    float hue;

    if (saturation == 0.0f)
    {
        hue = 0.0f;
    }
    else
    {
        if (red == value)
            hue = (green - blue) / delta;
        else if (green == value)
            hue = (blue - red) / delta + 2.0f;
        else
            hue = (red - green) / delta + 4.0f;

        hue *= 0.16666667f;
        if (hue < 0.0f)
            hue += 1.0f;
    }

    hsv->n[0] = (unsigned __int16)(hue * 65536.0f);
    hsv->n[1] = (unsigned __int16)(saturation * 65535.0f);
    hsv->n[2] = (unsigned __int16)(value * 65535.0f);
    return hsv;
}
