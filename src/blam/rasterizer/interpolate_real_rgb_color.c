/* interpolate_real_rgb_color @0x836E5E48 — move each channel of `current` toward `desired`, limiting the
 * per-call change to +-maximum_speed so colors ease rather than snap. */

#include "headers/real_rgb_color.h"

void interpolate_real_rgb_color(real_rgb_color *current, const real_rgb_color *desired, float maximum_speed)
{
    for ( int channel = 0; channel < 3; channel++ )
    {
        float delta = desired->n[channel] - current->n[channel];
        if ( delta < -maximum_speed )
            delta = -maximum_speed;
        else if ( delta > maximum_speed )
            delta = maximum_speed;
        current->n[channel] = current->n[channel] + delta;
    }
}
