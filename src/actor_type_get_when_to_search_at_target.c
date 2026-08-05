#include <stdint.h>
#include "headers/actor_type_definition.h"

int16_t actor_type_get_when_to_search_at_target(int16_t actor_type)
{
    return actor_type_definitions[actor_type]->when_to_search_at_target;
}
