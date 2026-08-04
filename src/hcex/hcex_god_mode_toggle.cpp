/* hcex_god_mode_toggle @ 0x836EAEF8
   Toggles the deathless-player (god-mode) cheat flag. In the binary the negate
   is emitted as (cntlzw(x) & 0x20) != 0, i.e. "x == 0"; a plain logical NOT is
   the equivalent for a 0/1 flag. */

#include "../headers/cheat_globals.h"

extern "C" void hcex_god_mode_toggle(void)
{
    cheat.deathless_player = !cheat.deathless_player;
}
