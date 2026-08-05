/* scenario_get_animation_by_name @0x8381F870 — find a recorded_animations entry (64-byte stride) by
 * name, returning its index or -1. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/recorded_animation_definition.h"

extern int stricmp(const char *a, const char *b);

int16_t scenario_get_animation_by_name(scenario *scenario, const char *animation_name)
{
    if ( scenario->recorded_animations.count <= 0 )
        return -1;

    int16_t i = 0;

    while ( stricmp(((recorded_animation_definition *)scenario->recorded_animations.address)[i].name, animation_name) )
    {
        if ( ++i >= scenario->recorded_animations.count )
            return -1;
    }
    return i;
}
