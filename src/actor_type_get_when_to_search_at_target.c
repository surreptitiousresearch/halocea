#include "headers/actor_type_definition.h"

int actor_type_get_when_to_search_at_target(__int16 actor_type)
{
    return (unsigned __int16)actor_type_definitions[actor_type]->when_to_search_at_target;
}
