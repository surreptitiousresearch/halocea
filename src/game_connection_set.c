/* game_connection_set @0x83688E18 — set the active game connection mode (none / client / server / film). */

#include <stdint.h>
#include "headers/main_globals.h"

void game_connection_set(int16_t new_connection)
{
    main_globals.connection = new_connection;
}
