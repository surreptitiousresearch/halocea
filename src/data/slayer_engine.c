/* slayer_engine @ 0x841857C8 -- second-order .data; game_engine reconstructed from binary bytes. */
#include <stdint.h>
#include "../headers/game_engine.h"

#include "headers/game_statistics.h"
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
extern uint8_t slayer_engine_allow_pick_up(int unit_index, int item_index);
extern uint8_t slayer_engine_display_score(int player_index, int message, int message_data, uint16_t *buffer, int buffer_size);
extern void slayer_engine_dispose(void); /* prototype not in DB */
extern void slayer_engine_dispose_from_old_map(void); /* prototype not in DB */
extern void slayer_engine_game_ending(void); /* prototype not in DB */
extern void slayer_engine_game_starting(void); /* prototype not in DB */
extern void slayer_engine_handle_client_message(int player_index, void *encoded_message, int16_t encoded_message_size);
extern void slayer_engine_handle_server_message(void *encoded_message, int16_t encoded_message_size);
extern uint8_t slayer_engine_initialize_for_new_map(void);
extern void slayer_engine_player_added(int player_index);
extern void slayer_engine_player_damaged_player(int killing_player_index, int dead_player_index, uint8_t friendly_fire);
extern void slayer_engine_player_killed_player(int killing_player_index, int killing_object_index, int dead_player_index, uint8_t friendly_fire);
extern void slayer_engine_post_rasterize(void); /* prototype not in DB */
extern void slayer_engine_pregame_post_rasterize(void); /* prototype not in DB */
extern void slayer_engine_prespawn_player_update(int player_index);
extern void slayer_engine_replicate_game_mode_state_from_network(message_delta_processor_header *header, network_game_client *client);
extern void slayer_engine_replicate_game_mode_state_to_network(message_delta_processor_mode mode, const int machine_index);
extern void slayer_engine_statistics_append(game_statistics *permanent_statistics, game_statistics *game_statistics);
extern void slayer_engine_update(void); /* prototype not in DB */
extern int slayer_get_score(int player_index, get_score_type get_score_type);
extern uint16_t * slayer_get_score_header_string(uint16_t *buffer);
extern uint16_t * slayer_get_score_string(int player_index, uint16_t *buffer);
extern int slayer_get_team_score(int team_index);
extern uint16_t * slayer_get_team_score_string(int team_index, uint16_t *buffer);
extern void slayer_player_changed_team(int player_index, uint8_t new_team);
extern void slayer_player_update(int player_index);
extern uint8_t slayer_test_flag(int flag);

struct game_engine slayer_engine = {
    .name = "slayer",
    .type = 2,
    .dispose = &slayer_engine_dispose,
    .initialize = &slayer_engine_initialize_for_new_map,
    .dispose_from_old_map = &slayer_engine_dispose_from_old_map,
    .player_added = &slayer_engine_player_added,
    .game_ending = &slayer_engine_game_ending,
    .game_starting = &slayer_engine_game_starting,
    .statistics_append = &slayer_engine_statistics_append,
    .handle_client_message = &slayer_engine_handle_client_message,
    .handle_server_message = &slayer_engine_handle_server_message,
    .pregame_post_rasterize = &slayer_engine_pregame_post_rasterize,
    .post_rasterize = &slayer_engine_post_rasterize,
    .player_update = &slayer_player_update,
    .update = &slayer_engine_update,
    .get_score = &slayer_get_score,
    .get_team_score = &slayer_get_team_score,
    .get_score_string = &slayer_get_score_string,
    .get_score_header_string = &slayer_get_score_header_string,
    .get_team_score_string = &slayer_get_team_score_string,
    .allow_pick_up = &slayer_engine_allow_pick_up,
    .player_damaged_player = &slayer_engine_player_damaged_player,
    .player_killed_player = &slayer_engine_player_killed_player,
    .rasterize_score = &slayer_engine_display_score,
    .prespawn_player_update = &slayer_engine_prespawn_player_update,
    .game_engine_test_flag = &slayer_test_flag,
    .replicate_game_mode_state_to_network = &slayer_engine_replicate_game_mode_state_to_network,
    .replicate_game_mode_state_from_network = &slayer_engine_replicate_game_mode_state_from_network,
    .player_changed_team = &slayer_player_changed_team,
};
