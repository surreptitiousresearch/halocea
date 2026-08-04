/* real_argb_color_to_pixel32 @0x836A3AE0 — pack a float ARGB color into a 32-bit A8R8G8B8 pixel. */

#include "headers/real_argb_color.h"

unsigned int real_argb_color_to_pixel32(const real_argb_color *color)
{
    unsigned int alpha_byte = (unsigned int)(color->n[0] * 255.0f) & 0xFF;
    unsigned int r_byte = (unsigned int)(color->n[1] * 255.0f) & 0xFF;
    unsigned int g_byte = (unsigned int)(color->n[2] * 255.0f) & 0xFF;
    unsigned int b_byte = (unsigned int)(color->n[3] * 255.0f) & 0xFF;

    return (alpha_byte << 24) | (r_byte << 16) | (g_byte << 8) | b_byte;
}
