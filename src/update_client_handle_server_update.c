/* update_client_handle_server_update @0x836F6870 — client-side receipt of one numbered game update
 * from the server. Stores the update in the client's ring of pending updates, and if it advances the
 * latest-received sequence number, fills any skipped slots with empty updates (number_of_actions = -1)
 * while bumping the fake-update counter when actively connected. Finally, while connected and not in
 * the main menu, records the tick into the demo/film, ending the recording if the writer is full. */

#include <stdint.h>
#include <string.h>
#include "headers/game_update.h"
#include "headers/update.h"
#include "headers/update_client_globals.h"
#include "headers/game_connection.h"

#include "headers/network_game_client.h"
#include "headers/network_game_server.h"
#include "headers/player_action.h"
extern update *update_client_get_update(int update_number);
extern int16_t game_connection(void);
extern network_game_client *global_network_game_client_get(void);
extern network_game_server *global_network_game_server_get(void);
extern uint8_t main_menu_is_active(void);
extern uint32_t get_random_seed(void);
extern uint8_t save_film_record_one_game_tick(unsigned int update_number, unsigned int random_seed, int16_t player_count, player_action *action_updates);
extern void save_film_end_recording(void);

extern int s_fake_update_number;

void update_client_handle_server_update(game_update *new_update, int update_number)
{
    update *slot = update_client_get_update(update_number);
    if (slot)
    {
        slot->update_number = update_number;
        game_update *data = &slot->data;
        memcpy(&slot->data, new_update, sizeof(slot->data));
        if (update_number > update_client_globals.latest_update_number_received)
        {
            if (update_client_globals.latest_update_number_received + 1 < update_number)
            {
                int missing = update_number - (update_client_globals.latest_update_number_received + 1);
                do
                {
                    if (game_connection() != _game_connection_network_server && game_connection() && game_connection() != _game_connection_film_playback)
                        ++s_fake_update_number;
                    --missing;
                    data->number_of_actions = -1;
                } while (missing);
            }
            update_client_globals.latest_update_number_received = update_number;
        }
    }
    else if (!global_network_game_client_get() || !global_network_game_server_get())
    {
        main_menu_is_active();
    }

    if ((unsigned int)game_connection() < _game_connection_film_playback && !main_menu_is_active())
    {
        uint16_t number_of_actions = new_update->number_of_actions;
        unsigned int random_seed = get_random_seed();
        if (!save_film_record_one_game_tick(update_number, random_seed, number_of_actions,
                                            new_update->actions))
            save_film_end_recording();
    }
}
