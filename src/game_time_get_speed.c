/* game_time_get_speed @0x8369A970 — current game-clock speed multiplier. Network clients and servers
 * always run at 1.0; standalone play uses the configurable global speed.
 *
 * DEVIATION: the decompiler routes the float result through a double (soft-float ABI artifact). */

#include <stdint.h>
#include "headers/game_time_globals.h"
#include "headers/game_connection.h"

extern int16_t game_connection(void);

float game_time_get_speed(void)
{
    if ( game_connection() == _game_connection_network_client || game_connection() == _game_connection_network_server )
        return 1.0f;
    return game_time_globals->speed;
}
