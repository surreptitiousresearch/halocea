/* sv_ban @0x83766ED0 — server console command: ban the player matching a name/hash string. Rejected
 * off the server.
 *
 * DEVIATION: the binary issues three sequential calls to the symbol resolved as
 * global_network_game_server_get (likely identical-code-folded distinct server operations — lookup,
 * disconnect, ban-list add); results are discarded. Reproduced verbatim. */

#include <stdint.h>
#include "headers/real_argb_color.h"
#include "headers/game_connection.h"
#include "headers/network_player.h"
#include "headers/blam_data_globals.h"

#include "headers/network_player.h"
#include "headers/network_player.h"
extern int16_t game_connection(void);
extern network_player * find_player_matching_string(const char *str);
extern network_game_server *global_network_game_server_get(void);
extern void ship_terminal_printf(const real_argb_color *color, const char *format, ...);

void sv_ban(const char *str)
{
    if ( game_connection() == _game_connection_network_server )
    {
        if ( find_player_matching_string(str) )
        {
            global_network_game_server_get();
            global_network_game_server_get();
            global_network_game_server_get();
        }
    }
    else
    {
        ship_terminal_printf(global_real_argb_white, "sv_ban is a server-only function!");
    }
}
