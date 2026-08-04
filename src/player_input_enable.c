/* player_input_enable @0x836A8450 — enable or disable player input by setting the inhibit flag to the
 * logical negation of `enable`. */

#include <stdint.h>
#include "headers/players_globals.h"

void player_input_enable(uint8_t enable)
{
    players_globals->input_inhibited = (enable == 0);
}
