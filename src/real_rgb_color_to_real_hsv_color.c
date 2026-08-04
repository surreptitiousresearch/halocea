/* real_rgb_color_to_real_hsv_color @0x8377D490 — standard RGB→HSV conversion. Value is the channel
 * maximum; saturation is chroma/value; hue is computed from whichever channel is the maximum and
 * normalised to [0,1) (the 1/6 scale folds the 0..6 sextant hue into a fraction). */

#include "headers/real_rgb_color.h"
#include "headers/real_hsv_color.h"

real_hsv_color *real_rgb_color_to_real_hsv_color(const real_rgb_color *rgb, real_hsv_color *hsv)
{
    float red   = rgb->n[0];
    float green = rgb->n[1];
    float blue  = rgb->n[2];

    float value = (green <= blue) ? blue : green;
    if (red > value)
        value = red;

    float minimum = (green <= blue) ? green : blue;
    if (red <= minimum)
        minimum = red;

    hsv->n[2] = value;
    float delta = value - minimum;

    float saturation = (value == 0.0f) ? 0.0f : (value - minimum) / value;
    hsv->n[1] = saturation;

    if (saturation == 0.0f)
    {
        hsv->n[0] = 0.0f;
    }
    else
    {
        if (red == value)
            hsv->n[0] = (green - blue) / delta;
        else if (green == value)
            hsv->n[0] = (blue - red) / delta + 2.0f;
        else
            hsv->n[0] = (red - green) / delta + 4.0f;

        float hue = hsv->n[0] * 0.16666667f;
        hsv->n[0] = hue;
        if (hue < 0.0f)
            hsv->n[0] = hue + 1.0f;
    }
    return hsv;
}
