/* network_handle_replication @ 0x83688050 — Blam engine.
 * When the current connection mode (game_connection) is the server (2), returns
 * the global network game server; otherwise returns the connection value itself.
 * DEVIATION: the disasm returns game_connection()'s raw value in the non-server
 * path (reinterpreted as the pointer-typed result); reproduced faithfully. */

#include <stdint.h>
#include "headers/network_game_server.h"

extern int16_t game_connection(void);
extern network_game_server *global_network_game_server_get(void);

network_game_server *network_handle_replication(void)
{
    int connection = game_connection();
    if ((short)connection == 2)
        return global_network_game_server_get();
    return (network_game_server *)connection;
}
