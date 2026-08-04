#include "headers/blam_data_globals.h"
/* network_game_set_number_of_games_played @0x83756690 — stamps the games-played counter into a fixed
 * low-memory address (0x548), gated on the server and/or client being active.
 *
 * DEVIATION: disasm_range (0x83756690-0x837566B8) and the decompiler both independently show `stw r3, 0x548`
 * with no base register (d(0) addressing) — a literal absolute address, not an offset into
 * global_network_game_server/global_network_game_client (those pointers are only used for the null checks). */


void network_game_set_number_of_games_played(int number_of_games_played)
{
    if (global_network_game_server)
        *(int *)0x548 = number_of_games_played;
    if (global_network_game_client)
        *(int *)0x548 = number_of_games_played;
}
