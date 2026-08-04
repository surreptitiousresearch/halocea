/* interpolate_real_rgb_color_0 @0x8370405C — duplicate/sibling of interpolate_real_rgb_color.c (identical
 * per-channel clamped-delta logic, distinct address). */

#include "headers/real_rgb_color.h"

void interpolate_real_rgb_color_0(real_rgb_color *current, real_rgb_color *desired, float maximum_speed)
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
