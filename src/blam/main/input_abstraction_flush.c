/* input_abstraction_flush @0x83724A48 — flush pending input edge state: mark all controls idle, clear
 * the aux-button tick counters, and zero player 0's abstracted input state. */

#include <string.h>
#include "headers/input_abstraction_globals.h"
#include "headers/auxbutton.h"


void input_abstraction_flush(void)
{
    input_abstraction_globals.all_idle = 1;
    input_abstraction_globals.auxbutton_ticks[_auxbutton_console] = 0;
    input_abstraction_globals.auxbutton_ticks[_auxbutton_pause] = 0;
    input_abstraction_globals.auxbutton_ticks[_auxbutton_screenshot] = 0;

    /* recovered: byte-cursor walk from &player_control_settings[3].invert_look+4 (9 dwords, 0x90..0xB4)
     * -> input_state[0] */
    memset(&input_abstraction_globals.input_state[0], 0, sizeof(input_abstraction_globals.input_state[0]));
}
