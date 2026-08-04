/* placement_data_set_change_color @0x836A8FF8 — set all four change-color channels of an
 * object_placement_data to the same rgb color. */

#include "headers/object_placement_data.h"
#include "headers/real_rgb_color.h"

void placement_data_set_change_color(object_placement_data *placement_data, const real_rgb_color *color)
{
    placement_data->change_colors[0] = *color;
    placement_data->change_colors[1] = *color;
    placement_data->change_colors[2] = *color;
    placement_data->change_colors[3] = *color;
}
