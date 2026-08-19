/* input_abstraction_auxbutton_down @ 0x83724DC8 — held-tick count for an aux button
 * (1 on the frame it first goes down). */

#include <stdint.h>
#include "headers/input_abstraction_globals.h"

uint8_t input_abstraction_auxbutton_down(int16_t button)
{
    return input_abstraction_globals.auxbutton_ticks[button];
}
