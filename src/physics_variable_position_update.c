/* physics_variable_position_update @0x83810660 */
#include <stdint.h>
#include "headers/physics_variable_position.h"

void physics_variable_position_update(float *position, const physics_variable_position *definition, uint8_t cyclical_position, float speed)
{
    float new_position = *position + speed;
    *position = new_position;

    if ( new_position < definition->minimum_position )
    {
        if ( cyclical_position )
        {
            *position = (definition->maximum_position - definition->minimum_position) + new_position;
            return;
        }
        *position = definition->minimum_position;
        return;
    }

    if ( new_position <= definition->maximum_position )
        return;

    if ( cyclical_position )
        *position = new_position - (definition->maximum_position - definition->minimum_position);
    else
        *position = definition->maximum_position;
}
