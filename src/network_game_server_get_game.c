/* network_game_server_get_game @0x83756618 — retail stub: unconditionally returns 0.
 *
 * THE FOLDED-NULL BASE (re-derived from the binary 2026-08-07; canonical note for the whole family).
 * All three session-data accessors are two-instruction stubs in this build:
 *     network_game_server_get_game @0x83756618  li r3, 0 ; blr
 *     network_game_client_get_game @0x836918C8  li r3, 0 ; blr
 *     network_game_get_game        @0x837566F0  li r3, 0 ; blr
 * so MSVC constant-folds the NULL into every caller and `game->players[i]` compiles to the bare
 * displacement `0x142 + 32*i`. Thirteen functions therefore address `network_game_data` members at
 * literal low addresses, and it reads like a base our reconstruction dropped. It is not: the base is
 * absent in the MACHINE CODE, which is what those TUs reproduce. The decisive evidence is the raw
 * D-form encoding, since IDA prints `lwz r3, 0x544` for rA=0 and `lwz r3, 0x544(r11)` otherwise:
 *     teamplay_get_team_info      0x83802B38  3BE00160  addi r31, r0, 0x160   rA = r0
 *     sv_players                  0x83765E34  3BE00142  addi r31, r0, 0x142   rA = r0
 *     network_game_get_random_seed 0x83756AA0 80600544  lwz  r3,  0x544(r0)   rA = r0
 * versus the one member of the family that DOES carry a base, because its caller supplies it:
 *     network_game_update_player  0x83783140  add  r11, r11, r4  (r4 = the `game` parameter)
 *                                 0x83783144  addi r3,  r11, 0x142
 * That contrast is what makes the check non-vacuous. The surviving `bl global_network_game_*_get`
 * calls are the non-foldable half of the same expression and their results are genuinely dead.
 * Corroboration: network_game_client_local_player_quit's `addic. r11, r11, 0x142` is the original
 * `if (player != NULL)` with the folded base still inside it.
 *
 * Every offset in the family is a DB-named `network_game_data` member (types_members): players[32]
 * @0x142 (element `network_player`, DB size 1024/32 = 32-byte stride), maximum_players @0x13D,
 * network_game_random_seed @0x544, number_of_games_played @0x548. */

#include "headers/network_game_data.h"
#include "headers/network_game_server.h"

network_game_data * network_game_server_get_game(network_game_server *server)
{
    return 0;
}
