/* sv_players @0x83765E08 — server console command: print the number, team, and name of every connected
 * network player. Server-only. The network player records live at fixed low-memory addresses
 * (0x142..0x542, 32-byte stride); reproduced verbatim as the disassembly addresses them absolutely. */

#include <stdint.h>
#include "headers/network_player.h"
#include "headers/real_argb_color.h"
#include "headers/game_connection.h"
#include "headers/blam_data_globals.h"

#include "headers/network_game_server.h"
extern int16_t game_connection(void);
extern network_game_server *global_network_game_server_get(void);
extern int sprintf_0(char *string, const char *format, ...);
extern void ship_terminal_printf(const real_argb_color *color, const char *format, ...);
extern uint8_t network_player_is_valid(network_player *player);
extern char * wide_to_ascii(uint16_t *unicode, char *ascii, unsigned int ascii_length_bytes);

void sv_players(void)
{
    if ( game_connection() == _game_connection_network_server )
    {
        global_network_game_server_get();
        char line[288];
        sprintf_0(line, "Number  Team  Name");
        ship_terminal_printf(global_real_argb_darkgreen, line);
        for ( int address = 0x142; address < 0x542; address += 32 )
        {
            if ( network_player_is_valid((network_player *)address) )
            {
                network_player *player_record = (network_player *)address;
                char name[16];
                wide_to_ascii((unsigned __int16 *)address, name, 0xCu);
                sprintf_0(line, "%-3d     %-1d     %-32s", player_record->machine_index + 1, player_record->team_index, name);
                ship_terminal_printf(global_real_argb_green, line);
            }
        }
    }
    else
    {
        ship_terminal_printf(global_real_argb_white, "sv_players is a server-only function!");
    }
}
