#include <stdint.h>
#include "headers/widget_type_definition.h"

widget_type_definition * widget_type_definition_get(int16_t type)
{
    return &widget_type_definitions[type];
}
