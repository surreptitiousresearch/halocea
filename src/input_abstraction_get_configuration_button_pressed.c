#include "headers/input_abstraction_globals.h"

void input_abstraction_get_configuration_button_pressed(input_abstraction_input_button *button)
{
    *button = input_abstraction_globals.configuration_button_pressed;
}
