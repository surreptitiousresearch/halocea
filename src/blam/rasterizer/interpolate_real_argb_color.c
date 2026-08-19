/* interpolate_real_argb_color @0x836E5EF0 — move each of the four channels of `current` toward `desired`,
 * limiting the per-call change to +-maximum_speed so colors ease rather than snap. */

#include "headers/real_argb_color.h"

void interpolate_real_argb_color(real_argb_color *current, const real_argb_color *desired, float maximum_speed)
{
    for ( int channel = 0; channel < 4; channel++ )
    {
        float delta = desired->n[channel] - current->n[channel];
        if ( delta < -maximum_speed )
            delta = -maximum_speed;
        else if ( delta > maximum_speed )
            delta = maximum_speed;
        current->n[channel] = current->n[channel] + delta;
    }
}
