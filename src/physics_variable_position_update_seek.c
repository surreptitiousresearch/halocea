/* physics_variable_position_update_seek @0x83810998 — steps *position one tick (of magnitude `speed`)
 * toward desired_position, honoring the definition's [minimum, maximum] range. When cyclical, the
 * position wraps around the range and always takes the shorter way around. Returns 1 once the target is
 * reached (and snaps *position exactly to desired_position), 0 while still moving. */

#include <stdint.h>
#include "headers/physics_variable_position.h"

extern double __fabs(double x);

int physics_variable_position_update_seek(float *position, const physics_variable_position *definition, uint8_t cyclical_position, float desired_position, float speed)
{
    int reached = 1;

    float delta = desired_position - *position;
    if (delta != 0.0f)
    {
        if (cyclical_position
            && __fabs(delta) > (definition->maximum_position - definition->minimum_position) * 0.5f)
        {
            delta = -delta;
        }

        float direction = delta <= 0.0f ? -1.0f : 1.0f;
        float stepped = direction * speed + *position;
        *position = direction * speed + *position;

        if (stepped >= definition->minimum_position)
        {
            if (stepped > definition->maximum_position)
            {
                float clamped = definition->maximum_position;
                if (cyclical_position)
                    clamped = stepped - (definition->maximum_position - definition->minimum_position);
                *position = clamped;
            }
        }
        else if (cyclical_position)
        {
            *position = (definition->maximum_position - definition->minimum_position) + stepped;
        }
        else
        {
            *position = definition->minimum_position;
        }

        float remaining = desired_position - *position;
        float new_direction = remaining;
        if (remaining != 0.0f)
        {
            if (cyclical_position
                && __fabs(remaining) > (definition->maximum_position - definition->minimum_position) * 0.5f)
            {
                new_direction = -remaining;
            }
            new_direction = new_direction <= 0.0f ? -1.0f : 1.0f;
        }

        if (direction == new_direction)
            reached = 0;
    }

    if (reached)
        *position = desired_position;
    return reached;
}
