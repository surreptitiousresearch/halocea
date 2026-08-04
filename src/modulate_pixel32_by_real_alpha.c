/* modulate_pixel32_by_real_alpha @0x83730670 — scale a packed 0xAARRGGBB pixel's alpha channel by a
 * float factor, leaving RGB untouched. */

#include <stdint.h>

uint32_t modulate_pixel32_by_real_alpha(uint32_t argb, float alpha)
{
    uint32_t source_alpha = argb >> 24;
    uint32_t scaled_alpha = (uint32_t)((float)source_alpha * alpha);
    return (scaled_alpha << 24) | (argb & 0xFFFFFF);
}
