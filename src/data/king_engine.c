/* king_engine @ 0x84186AF0 -- second-order .data; game_engine reconstructed from binary bytes. */
#include <stdint.h>
#include "../headers/game_engine.h"

#include "headers/game_statistics.h"
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
extern uint8_t king_engine_display_score(int player_index, int message, int message_data, uint16_t *buffer, int buffer_size);
extern void king_engine_dispose(void); /* prototype not in DB */
extern void king_engine_dispose_from_old_map(void); /* prototype not in DB */
extern void king_engine_game_ending(void); /* prototype not in DB */
extern void king_engine_game_starting(void); /* prototype not in DB */
extern uint8_t king_engine_goal_matches_player(int player_index, int goal_index);
extern void king_engine_handle_client_message(int player_index, void *encoded_message, int16_t encoded_message_size);
extern void king_engine_handle_server_message(void *encoded_message, int16_t encoded_message_size);
extern unsigned __int8 king_engine_initialize_for_new_map(void);
extern void king_engine_player_added(int player_index);
extern void king_engine_player_damaged_player(int killing_player_index, int dead_player_index, uint8_t friendly_fire);
extern void king_engine_player_killed_player(int killing_player_index, int killing_object_index, int dead_player_index, uint8_t friendly_fire);
extern void king_engine_player_update(int player_index);
extern void king_engine_post_rasterize(void); /* prototype not in DB */
extern void king_engine_pregame_post_rasterize(void); /* prototype not in DB */
extern void king_engine_prespawn_player_update(int player_index);
extern void king_engine_replicate_game_mode_state_from_network(message_delta_processor_header *header, network_game_client *client);
extern void king_engine_replicate_game_mode_state_to_network(message_delta_processor_mode mode, int machine_index);
extern void king_engine_statistics_append(game_statistics *permanent_statistics, game_statistics *game_statistics);
extern int king_engine_update(void); /* prototype not in DB */
extern int king_get_score(int player_index, get_score_type get_score_type);
extern uint16_t * king_get_score_header_string(uint16_t *buffer);
extern uint16_t * king_get_score_string(int player_index, uint16_t *buffer);
extern int king_get_team_score(int team_index);
extern uint16_t * king_get_team_score_string(int team_index, uint16_t *buffer);
extern void king_player_changed_team(int player_index, uint8_t new_team);

struct game_engine king_engine = {
    .name = "king",
    .type = 4,
    .dispose = &king_engine_dispose,
    .initialize = &king_engine_initialize_for_new_map,
    .dispose_from_old_map = &king_engine_dispose_from_old_map,
    .player_added = &king_engine_player_added,
    .game_ending = &king_engine_game_ending,
    .game_starting = &king_engine_game_starting,
    .statistics_append = &king_engine_statistics_append,
    .handle_client_message = &king_engine_handle_client_message,
    .handle_server_message = &king_engine_handle_server_message,
    .pregame_post_rasterize = &king_engine_pregame_post_rasterize,
    .post_rasterize = &king_engine_post_rasterize,
    .player_update = &king_engine_player_update,
    .update = &king_engine_update,
    .get_score = &king_get_score,
    .get_team_score = &king_get_team_score,
    .get_score_string = &king_get_score_string,
    .get_score_header_string = &king_get_score_header_string,
    .get_team_score_string = &king_get_team_score_string,
    .player_damaged_player = &king_engine_player_damaged_player,
    .player_killed_player = &king_engine_player_killed_player,
    .rasterize_score = &king_engine_display_score,
    .prespawn_player_update = &king_engine_prespawn_player_update,
    .goal_matches_player = &king_engine_goal_matches_player,
    .replicate_game_mode_state_to_network = &king_engine_replicate_game_mode_state_to_network,
    .replicate_game_mode_state_from_network = &king_engine_replicate_game_mode_state_from_network,
    .player_changed_team = &king_player_changed_team,
};
