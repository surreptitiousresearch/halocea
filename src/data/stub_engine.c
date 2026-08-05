/* stub_engine @ 0x841868B8 -- second-order .data; game_engine reconstructed from binary bytes. */
#include <stdint.h>
#include "../headers/game_engine.h"

#include "headers/game_statistics.h"
extern uint8_t stub_engine_allow_pick_up(int unit_index, int item_index);
extern void stub_engine_dispose(void); /* prototype not in DB */
extern void stub_engine_dispose_from_old_map(void); /* prototype not in DB */
extern void stub_engine_game_ending(void); /* prototype not in DB */
extern void stub_engine_game_starting(void); /* prototype not in DB */
extern void stub_engine_handle_client_message(int player_index, void *encoded_message, int16_t encoded_message_size);
extern void stub_engine_handle_server_message(void *encoded_message, int16_t encoded_message_size);
extern uint8_t stub_engine_initialize_for_new_map(void);
extern void stub_engine_player_added(int player_index);
extern void stub_engine_player_damaged_player(int killing_player_index, int dead_player_index, uint8_t friendly_fire);
extern void stub_engine_player_killed_player(int killing_player_index, int killing_object_index, int dead_player_index, uint8_t friendly_fire);
extern void stub_engine_post_rasterize(void); /* prototype not in DB */
extern void stub_engine_pregame_post_rasterize(void); /* prototype not in DB */
extern void stub_engine_statistics_append(game_statistics *permanent_statistics, game_statistics *game_statistics);
extern void stub_engine_update(void); /* prototype not in DB */

struct game_engine stub_engine = {
    .name = "stub",
    .type = 7,
    .dispose = &stub_engine_dispose,
    .initialize = &stub_engine_initialize_for_new_map,
    .dispose_from_old_map = &stub_engine_dispose_from_old_map,
    .player_added = &stub_engine_player_added,
    .game_ending = &stub_engine_game_ending,
    .game_starting = &stub_engine_game_starting,
    .statistics_append = &stub_engine_statistics_append,
    .handle_client_message = &stub_engine_handle_client_message,
    .handle_server_message = &stub_engine_handle_server_message,
    .pregame_post_rasterize = &stub_engine_pregame_post_rasterize,
    .post_rasterize = &stub_engine_post_rasterize,
    .update = &stub_engine_update,
    .allow_pick_up = &stub_engine_allow_pick_up,
    .player_damaged_player = &stub_engine_player_damaged_player,
    .player_killed_player = &stub_engine_player_killed_player,
};
