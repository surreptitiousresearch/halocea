/* network_game_client_end_frame @ 0x83756990 — end-of-frame client bookkeeping; if the
 * client object is gone, drop to a local connection and ensure the menu's player queues
 * exist, then tick the "trouble is brewing" connection-quality monitor. Always succeeds. */
#include <stdint.h>
#include "headers/game_connection.h"
#include "headers/blam_data_globals.h"
typedef struct network_game_client network_game_client;
extern void game_connection_set(int16_t new_connection);
extern void main_menu_ensure_player_queues_exist(void);
extern void trouble_is_brewing_update(void);

uint8_t network_game_client_end_frame(int ticks_to_apply_this_frames_update_to,
                                      uint8_t did_time_overflow_occur)
{
    if ( !global_network_game_client )
    {
        game_connection_set(_game_connection_local);
        main_menu_ensure_player_queues_exist();
    }
    trouble_is_brewing_update();
    return 1;
}
