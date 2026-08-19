/* hardware_character_cache_get_origin @0x83768BE8 — reads a cached hardware character's atlas origin. */

#include <stdint.h>
#include "headers/hardware_character_cache.h"

void hardware_character_cache_get_origin(int16_t hardware_character_index, int16_t *x0, int16_t *y0)
{
    hardware_character *character = &hardware_character_cache.characters[hardware_character_index];
    *x0 = character->x0;
    *y0 = character->y0;
}
