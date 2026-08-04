/* player_input_enabled @0x836A8470 — return whether player input is currently enabled (not inhibited). */

#include <stdint.h>
#include "headers/players_globals.h"

/* Caller actor_unit_control normalizes r3 via `clrlwi r11,r3,24` => 8-bit (Blam boolean) return. */
uint8_t player_input_enabled(void)
{
    return players_globals->input_inhibited == 0;
}
