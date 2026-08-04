/* oddball_engine @ 0x84185940 -- second-order .data; game_engine reconstructed from binary bytes. */
#include <stdint.h>
#include "../headers/game_engine.h"

#include "headers/game_statistics.h"
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/weapon_datum.h"
extern uint8_t oddball_engine_display_score(int player_index, int message, int message_data, uint16_t *buffer, int buffer_size);
extern void oddball_engine_dispose(void); /* prototype not in DB */
extern void oddball_engine_dispose_from_old_map(void); /* prototype not in DB */
extern void oddball_engine_game_ending(void); /* prototype not in DB */
extern void oddball_engine_game_starting(void); /* prototype not in DB */
extern void oddball_engine_handle_client_message(int player_index, void *encoded_message, int16_t encoded_message_size);
extern void oddball_engine_handle_server_message(void *encoded_message, int16_t encoded_message_size);
extern uint8_t oddball_engine_initialize_for_new_map(void);
extern void oddball_engine_player_added(int player_index);
extern void oddball_engine_player_damaged_player(int killing_player_index, int dead_player_index, uint8_t friendly_fire);
extern void oddball_engine_player_killed_player(int killing_player_index, int killing_object_index, int dead_player_index, uint8_t friendly_fire);
extern void oddball_engine_player_update(int player_index);
extern void oddball_engine_post_rasterize(void); /* prototype not in DB */
extern void oddball_engine_pregame_post_rasterize(void); /* prototype not in DB */
extern void oddball_engine_prespawn_player_update(int player_index);
extern void oddball_engine_replicate_game_mode_state_from_network(message_delta_processor_header *header, network_game_client *client);
extern void oddball_engine_replicate_game_mode_state_to_network(message_delta_processor_mode mode, int machine_index);
extern void oddball_engine_statistics_append(game_statistics *permanent_statistics, game_statistics *game_statistics);
extern void oddball_engine_update(void); /* prototype not in DB */
extern void oddball_engine_weapon_update(int weapon_index, weapon_datum *weapon);
extern int oddball_get_score(int player_index, get_score_type get_score_type);
extern uint16_t * oddball_get_score_header_string(uint16_t *buffer);
extern uint16_t * oddball_get_score_string(int player_index, uint16_t *buffer);
extern int oddball_get_team_score(int team_index);
extern uint16_t * oddball_get_team_score_string(int team_index, uint16_t *buffer);
extern void oddball_player_changed_team(int player_index, uint8_t new_team);
extern uint8_t oddball_test_flag(int flag);
extern uint8_t oddball_test_trait(int player_index, int trait);
extern void oddball_weapon_drop(int weapon_index);
extern uint8_t oddball_weapon_pickup(int weapon_index, int player_index);

struct game_engine oddball_engine = {
    .name = "oddball",
    .type = 3,
    .dispose = &oddball_engine_dispose,
    .initialize = &oddball_engine_initialize_for_new_map,
    .dispose_from_old_map = &oddball_engine_dispose_from_old_map,
    .player_added = &oddball_engine_player_added,
    .game_ending = &oddball_engine_game_ending,
    .game_starting = &oddball_engine_game_starting,
    .statistics_append = &oddball_engine_statistics_append,
    .handle_client_message = &oddball_engine_handle_client_message,
    .handle_server_message = &oddball_engine_handle_server_message,
    .pregame_post_rasterize = &oddball_engine_pregame_post_rasterize,
    .post_rasterize = &oddball_engine_post_rasterize,
    .player_update = &oddball_engine_player_update,
    .weapon_update = &oddball_engine_weapon_update,
    .weapon_pickup = &oddball_weapon_pickup,
    .weapon_drop = &oddball_weapon_drop,
    .update = &oddball_engine_update,
    .get_score = &oddball_get_score,
    .get_team_score = &oddball_get_team_score,
    .get_score_string = &oddball_get_score_string,
    .get_score_header_string = &oddball_get_score_header_string,
    .get_team_score_string = &oddball_get_team_score_string,
    .player_damaged_player = &oddball_engine_player_damaged_player,
    .player_killed_player = &oddball_engine_player_killed_player,
    .rasterize_score = &oddball_engine_display_score,
    .prespawn_player_update = &oddball_engine_prespawn_player_update,
    .game_engine_test_flag = &oddball_test_flag,
    .game_engine_test_trait = &oddball_test_trait,
    .replicate_game_mode_state_to_network = &oddball_engine_replicate_game_mode_state_to_network,
    .replicate_game_mode_state_from_network = &oddball_engine_replicate_game_mode_state_from_network,
    .player_changed_team = &oddball_player_changed_team,
};
