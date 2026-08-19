/* scenario_get_encounter_by_name @0x83800488 — linear search of scenario.ai_encounters (176-byte elements,
 * name at offset 0, compared case-insensitively over at most 32 chars) for a match; returns the element index
 * or -1. */

#include "headers/scenario.h"

extern int strnicmp(const char *a, const char *b, unsigned int n);

int scenario_get_encounter_by_name(scenario *scenario, const char *encounter_name)
{
    if ( scenario->ai_encounters.count <= 0 )
        return -1;

    int index = 0;
    for ( int offset = 0;
          strnicmp((const char *)scenario->ai_encounters.address + offset, encounter_name, 0x20u);
          offset += 176 )
    {
        if ( ++index >= scenario->ai_encounters.count )
            return -1;
    }
    return index;
}
