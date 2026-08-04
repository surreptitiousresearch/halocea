#include "headers/actor_type_definition.h"

actor_type_definition * actor_type_definition_get(int actor_type)
{
    return actor_type_definitions[actor_type];
}
