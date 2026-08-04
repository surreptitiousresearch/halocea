/* encounter_definition_get_squad_by_name @0x83800500 — linear search of encounter.squads (232-byte elements,
 * name at offset 0, compared case-insensitively over at most 32 chars) for a match; returns the squad index or
 * -1. */

#include "headers/encounter_definition.h"

extern int strnicmp(const char *a, const char *b, unsigned int n);

int encounter_definition_get_squad_by_name(encounter_definition *encounter, const char *squad_name)
{
    if ( encounter->squads.count <= 0 )
        return -1;

    int index = 0;
    for ( int offset = 0;
          strnicmp((const char *)encounter->squads.address + offset, squad_name, 0x20u);
          offset += 232 )
    {
        if ( ++index >= encounter->squads.count )
            return -1;
    }
    return index;
}
