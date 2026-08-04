/* game_connection @ 0x83688E20 — current connection mode (none / network client / server). */

#include <stdint.h>
#include "headers/main_globals.h"

int16_t game_connection(void)
{
    /* (uint16_t) models the lhz (zero-extend) the binary emits on the signed __int16 field. */
    return (uint16_t)main_globals.connection;
}
