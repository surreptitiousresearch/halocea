/* ai_create_mounted_weapons_for_unit @0x836E9210 — register a unit as hosting a mounted weapon so the AI
 * can later spawn/assign gunners for it. No-op before the AI is initialized for the map or once the
 * fixed 8-slot mounted-weapon table is full. */

#include "headers/ai_globals.h"

void ai_create_mounted_weapons_for_unit(int unit_index)
{
    if (ai_globals->ai_initialized_for_map)
    {
        int count = ai_globals->mounted_weapon_unit_count;
        if (count < 8)
        {
            ai_globals->mounted_weapon_unit_indices[count] = unit_index;
            ++ai_globals->mounted_weapon_unit_count;
        }
    }
}
