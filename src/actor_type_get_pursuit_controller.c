/* actor_type_get_pursuit_controller @0x837F8EC0 */
#include <stdint.h>
#include "headers/actor_type_definition.h"

uint8_t actor_type_get_pursuit_controller(int16_t actor_type)
{
    return actor_type_definitions[actor_type]->pursuit_controller;
}
