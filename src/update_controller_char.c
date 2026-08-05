#include <stdint.h>
#include "headers/vector_char_difference_data.h"
#include "headers/direction_playback_controller.h"

void update_controller_char(const vector_char_difference_data *event_data, direction_playback_controller *control)
{
    int16_t yaw = event_data->delta_yaw + control->yaw;

    control->yaw = yaw;
    if ( yaw <= 1000 )
    {
        if ( yaw < -1000 )
            control->yaw += 1000;
    }
    else
    {
        control->yaw = yaw - 1000;
    }
    control->pitch += event_data->delta_pitch;
}
