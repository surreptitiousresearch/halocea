/* multiplayer_game_swap_teams @0x83692968 — team-swap handler: only in a team game, locate the
 * network player owned by this local machine and event controller; if found, fetch the global
 * network game client (the actual swap request is issued through it — the tail call's result is
 * unused here on this build). Always reports handled. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/network_game_data.h"
#include "headers/network_player.h"
#include "headers/network_game_client.h"

extern network_game_data *network_game_get_game(void);
extern int16_t network_game_client_get_local_machine_index(void);
extern uint8_t network_player_is_valid(network_player *player);
extern network_game_client *global_network_game_client_get(void);

uint8_t multiplayer_game_swap_teams(widget_instance *local_player_data_widget, event_record *event, uint8_t *widget_deleted)
{
    network_game_data *game = network_game_get_game();
    if ( game && game->variant.universal_variant.teams == 1 )
    {
        int local_machine_index = network_game_client_get_local_machine_index();
        char found = 0;
        if ( local_machine_index != -1 )
        {
            for ( int i = 0; i < 32; ++i )
            {
                network_player *player = &game->players[i];
                if ( network_player_is_valid(player)
                    && player->machine_index == local_machine_index
                    && (uint8_t)player->controller_index == (uint8_t)event->controller_index )
                {
                    found = 1;
                    break;
                }
            }
        }
        if ( found )
            global_network_game_client_get();
    }
    return 1;
}
