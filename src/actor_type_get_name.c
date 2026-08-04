#include <stdint.h>
#include "headers/actor_type_definition.h"

const char * actor_type_get_name(int16_t actor_type)
{
    return actor_type_definitions[actor_type]->name;
}
