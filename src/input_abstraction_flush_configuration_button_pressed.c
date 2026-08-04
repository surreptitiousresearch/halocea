#include "headers/input_abstraction_globals.h"

void input_abstraction_flush_configuration_button_pressed(void)
{
    input_abstraction_globals.configuration_button_pressed.device_type = 0;
    input_abstraction_globals.configuration_button_pressed.button_type = 0;
    input_abstraction_globals.configuration_button_pressed.parameter = 0;
}
