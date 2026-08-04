#include "headers/hardware_character_cache.h"
#include "headers/font_character.h"

void flush_hardware_character(hardware_character *hardware_character)
{
    if (hardware_character->character)
    {
        hardware_character->character->hardware_character_index = -1;
        hardware_character->character = 0;
    }
}
