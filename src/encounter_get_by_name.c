/* encounter_get_by_name @0x837098C8 — linear search of the scenario's ai_encounters tag_block for an
 * encounter whose name (32-byte field at the start of each 176-byte element) matches, or -1. */

#include <string.h>
#include "headers/scenario.h"

int encounter_get_by_name(const char *encounter_name)
{
    if ( !global_scenario )
        return -1;

    if ( global_scenario->ai_encounters.count <= 0 )
        return -1;

    int index = 0;

    while ( strncmp((const char *)global_scenario->ai_encounters.address + index * 176, encounter_name, 0x20) != 0 )
    {
        if ( ++index >= global_scenario->ai_encounters.count )
            return -1;
    }

    return index;
}
