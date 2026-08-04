/* hardware_character_cache_get_bitmap @ 0x83768BD8
   Accessor for the text cache's backing bitmap. */

#include "headers/hardware_character_cache.h"

bitmap_data *hardware_character_cache_get_bitmap(void)
{
    return hardware_character_cache.bitmap;
}
