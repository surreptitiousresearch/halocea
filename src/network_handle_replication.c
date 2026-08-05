/* network_handle_replication @ 0x83688050 — Blam engine.
 * When the current connection mode (game_connection) is the network server, touch the global
 * network game server object; otherwise do nothing.
 * DEVIATION: the decompiler threaded r3 out of this function as a network_game_server* return.
 * The disasm has no r3 computation of its own — on the server path r3 is only whatever
 * global_network_game_server_get left behind (8368806C), and on the other path it is
 * game_connection's leftover value (8368805C); the `bne` at 83688068 jumps straight to the
 * epilogue. With no in-corpus consumer of r3, the return is a phantom: attested void. */

#include <stdint.h>

#include "headers/game_connection.h"
#include "headers/network_game_server.h"

extern int16_t game_connection(void);
extern network_game_server *global_network_game_server_get(void);

void network_handle_replication(void)
{
    if ( game_connection() == _game_connection_network_server )
        global_network_game_server_get();
}
