#include <stdint.h>
#include "headers/widget_globals.h"

extern uint8_t input_has_gamepad(int16_t gamepad_index);

void dim_unless_two_controllers(widget_instance *widget)
{
    int gamepad_count = 0;

    for ( int i = 0; i < 2; i++ )
    {
        if ( input_has_gamepad(i) )
            gamepad_count++;
    }

    widget->alpha_modifier = (gamepad_count < 2) ? 0.333f : 1.0f;
}
