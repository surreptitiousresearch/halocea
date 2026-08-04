#include <stdint.h>
#include "headers/object_type_definition.h"

int object_type_get_datum_size(int16_t object_type)
{
    return (unsigned __int16)object_type_definitions[object_type]->game_datum_size;
}
