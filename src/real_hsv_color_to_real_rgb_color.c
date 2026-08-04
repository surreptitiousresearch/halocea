/* real_hsv_color_to_real_rgb_color @0x8377D5F0 — standard HSV→RGB conversion (the inverse of
 * real_rgb_color_to_real_hsv_color). Hue is mapped into one of six sextants; saturation 0 yields a
 * pure grey at the value level.
 *
 * DEVIATION: the decompiler invents phantom integer args a3..a7 (uninitialised ABI scratch) and
 * renders the sextant int→float conversion as `(float)__SPAIR64__(0x82000000, v10)` — that magic pair
 * is just the PPC fcfid conversion of the integer sextant, i.e. `(float)sextant`. */

#include "headers/real_hsv_color.h"
#include "headers/real_rgb_color.h"

real_rgb_color *real_hsv_color_to_real_rgb_color(const real_hsv_color *hsv, real_rgb_color *rgb)
{
    float hue6 = hsv->n[0] * 6.0f;
    if (hsv->n[1] == 0.0f)
    {
        rgb->n[1] = hsv->n[2];
        rgb->n[0] = hsv->n[2];
        rgb->n[2] = hsv->n[2];
        return rgb;
    }

    unsigned int sextant = (int)hue6;
    if ((float)sextant > hue6)   /* floor toward -inf */
        --sextant;

    float saturation = hsv->n[1];
    float value = hsv->n[2];
    float fraction = hue6 - (float)sextant;
    float p = (1.0f - saturation) * value;
    float q = (1.0f - saturation * fraction) * value;
    float t = (1.0f - saturation * (1.0f - fraction)) * value;

    if (sextant <= 5)
    {
        switch (sextant)
        {
            case 0: rgb->n[0] = value; rgb->n[1] = t;     rgb->n[2] = p;     return rgb;
            case 1: rgb->n[0] = q;     rgb->n[1] = value; rgb->n[2] = p;     return rgb;
            case 2: rgb->n[0] = p;     rgb->n[1] = value; rgb->n[2] = t;     return rgb;
            case 3: rgb->n[0] = p;     rgb->n[1] = q;     rgb->n[2] = value; return rgb;
            case 4: rgb->n[0] = t;     rgb->n[1] = p;     rgb->n[2] = value; return rgb;
            default: rgb->n[0] = value; rgb->n[1] = p;    rgb->n[2] = q;     return rgb;  /* case 5 */
        }
    }
    return rgb;
}
