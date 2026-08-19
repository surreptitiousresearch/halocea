/* actor_type_get_swarm @0x837F8EE0 — return whether an actor type definition describes a swarm actor. */

#include <stdint.h>
#include "headers/actor_type_definition.h"

extern actor_type_definition *actor_type_definitions[];

uint8_t actor_type_get_swarm(int16_t actor_type)
{
    return actor_type_definitions[actor_type]->swarm;
}
