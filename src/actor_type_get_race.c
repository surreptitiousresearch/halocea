/* actor_type_get_race @0x837F8E40 — return the race bitmask of an actor type (from its definition). */

#include <stdint.h>
#include "headers/actor_type_definition.h"

uint16_t actor_type_get_race(int16_t actor_type)
{
    /* return type uint16_t already zero-extends the lhz-loaded 16-bit race bitmask — cast removed */
    return actor_type_definitions[actor_type]->race;
}
