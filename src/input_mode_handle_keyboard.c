#include "headers/input_abstraction_globals.h"

extern void *memset(void *dst, int value, unsigned int size);

void input_mode_handle_keyboard(void)
{
    /* recovered: byte-cursor walk from &player_control_settings[3].invert_look+4 (9 dwords, 0x90..0xB4)
     * -> input_state[0] */
    memset(&input_abstraction_globals.input_state[0], 0, sizeof(input_abstraction_globals.input_state[0]));
}
