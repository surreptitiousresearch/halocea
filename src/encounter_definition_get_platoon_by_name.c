/* encounter_definition_get_platoon_by_name @0x83800578 — linear search of encounter.platoons (172-byte
 * elements, name at offset 0, compared case-insensitively over at most 32 chars) for a match; returns the
 * platoon index or -1. */

#include "headers/encounter_definition.h"

extern int strnicmp(const char *a, const char *b, unsigned int n);

int encounter_definition_get_platoon_by_name(encounter_definition *encounter, const char *platoon_name)
{
    if ( encounter->platoons.count <= 0 )
        return -1;

    int index = 0;
    for ( int offset = 0;
          strnicmp((const char *)encounter->platoons.address + offset, platoon_name, 0x20u);
          offset += 172 )
    {
        if ( ++index >= encounter->platoons.count )
            return -1;
    }
    return index;
}
