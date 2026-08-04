/* real_rgb_color_brightness @ 0x8377D0D0 — perceptual luminance of an RGB color using the standard
 * Rec.601 luma weights (0.299 R, 0.587 G, 0.114 B). */

#include "headers/real_rgb_color.h"

float real_rgb_color_brightness(const real_rgb_color *color)
{
    return color->red * 0.29899999f + (color->green * 0.58700001f + color->blue * 0.114f);
}
