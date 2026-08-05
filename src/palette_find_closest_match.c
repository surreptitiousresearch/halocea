#include <stdint.h>
/* palette_find_closest_match @0x836F4B38 — nearest palette index for an ARGB color by squared RGB
 * distance over a (zero-terminated, up to 256-entry) palette. Mostly-transparent colors (alpha byte <=
 * 0x80 as an unsigned dword compare of the masked value) map to the reserved transparent index 255. */

uint8_t palette_find_closest_match(const unsigned int *palette, unsigned int color)
{
    if ( (color & 0xFF000000) <= 0x80000000 )
        return 255;

    uint8_t closest_index = -1;
    int closest_distance = 0;

    for ( int16_t i = 0; i < 256; i++ )
    {
        unsigned int entry = palette[i];
        if ( !entry )
            break;

        int delta_red = (int)((entry >> 16) & 0xFF) - (int)((color >> 16) & 0xFF);
        if ( delta_red < 0 )
            delta_red = -delta_red;
        int delta_green = (int)((entry >> 8) & 0xFF) - (int)((color >> 8) & 0xFF);
        if ( delta_green < 0 )
            delta_green = -delta_green;
        int delta_blue = (int)(entry & 0xFF) - (int)(color & 0xFF);
        if ( delta_blue < 0 )
            delta_blue = -delta_blue;

        int distance = delta_blue * delta_blue + delta_green * delta_green + delta_red * delta_red;
        if ( !i || closest_distance > distance )
        {
            closest_distance = distance;
            closest_index = i;
        }
    }
    return closest_index;
}
