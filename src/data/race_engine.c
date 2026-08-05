/* race_engine @ 0x84186958 -- second-order .data; game_engine reconstructed from binary bytes. */
#include <stdint.h>
#include "../headers/game_engine.h"

#include "headers/game_statistics.h"
#include "headers/weapon_datum.h"
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
extern int __fastcall race_engine_did_player_win(int player_index);
extern uint8_t race_engine_display_score(int player_index, int message, int message_data, uint16_t *buffer, int buffer_size);
extern void race_engine_dispose(void); /* prototype not in DB */
extern void race_engine_dispose_from_old_map(void); /* prototype not in DB */
extern void race_engine_game_ending(void); /* prototype not in DB */
extern void race_engine_game_starting(void); /* prototype not in DB */
extern int race_engine_get_score(int player_index, get_score_type get_score_type);
extern int race_engine_get_team_score(int team_index);
extern void race_engine_handle_client_message(int player_index, void *encoded_message, int16_t encoded_message_size);
extern void race_engine_handle_server_message(void *encoded_message, int16_t encoded_message_size);
extern uint8_t race_engine_initialize_for_new_map(void);
extern void race_engine_player_added(int player_index);
extern void race_engine_player_damaged_player(int killing_player_index, int dead_player_index, uint8_t friendly_fire);
extern void race_engine_player_killed_player(int killing_player_index, int killing_object_index, int dead_player_index, uint8_t friendly_fire);
extern void race_engine_player_removed(int player_index);
extern void race_engine_player_update(int player_index);
extern void race_engine_post_rasterize(void); /* prototype not in DB */
extern uint8_t __fastcall race_engine_postspawn_player_update(int player_index);
extern void race_engine_pregame_post_rasterize(void); /* prototype not in DB */
extern void race_engine_prespawn_player_update(int player_index);
extern void race_engine_replicate_game_mode_state_from_network(message_delta_processor_header *header, network_game_client *client);
extern void race_engine_replicate_game_mode_state_to_network(message_delta_processor_mode mode, int machine_index);
extern void race_engine_statistics_append(game_statistics *permanent_statistics, game_statistics *game_statistics);
extern void race_engine_update(void); /* prototype not in DB */
extern void race_engine_weapon_update(int weapon_index, weapon_datum *weapon);
extern uint16_t * race_get_score_header_string(uint16_t *buffer);
extern uint16_t * race_get_score_string(int player_index, uint16_t *buffer);
extern uint16_t * race_get_team_score_string(int team_index, uint16_t *buffer);
extern uint8_t race_goal_matches_player(int player_index, int goal_index);
extern void race_player_changed_team(int player_index, uint8_t new_team);

struct game_engine race_engine = {
    .name = "race",
    .type = 5,
    .dispose = &race_engine_dispose,
    .initialize = &race_engine_initialize_for_new_map,
    .dispose_from_old_map = &race_engine_dispose_from_old_map,
    .player_added = &race_engine_player_added,
    .player_removed = &race_engine_player_removed,
    .game_ending = &race_engine_game_ending,
    .game_starting = &race_engine_game_starting,
    .statistics_append = &race_engine_statistics_append,
    .handle_client_message = &race_engine_handle_client_message,
    .handle_server_message = &race_engine_handle_server_message,
    .pregame_post_rasterize = &race_engine_pregame_post_rasterize,
    .post_rasterize = &race_engine_post_rasterize,
    .player_update = &race_engine_player_update,
    .weapon_update = &race_engine_weapon_update,
    .update = &race_engine_update,
    .get_score = &race_engine_get_score,
    .get_team_score = &race_engine_get_team_score,
    .get_score_string = &race_get_score_string,
    .get_score_header_string = &race_get_score_header_string,
    .get_team_score_string = &race_get_team_score_string,
    .player_damaged_player = &race_engine_player_damaged_player,
    .player_killed_player = &race_engine_player_killed_player,
    .rasterize_score = &race_engine_display_score,
    .prespawn_player_update = &race_engine_prespawn_player_update,
    .postspawn_player_update = &race_engine_postspawn_player_update,
    .goal_matches_player = &race_goal_matches_player,
    .game_engine_did_player_win = &race_engine_did_player_win,
    .replicate_game_mode_state_to_network = &race_engine_replicate_game_mode_state_to_network,
    .replicate_game_mode_state_from_network = &race_engine_replicate_game_mode_state_from_network,
    .player_changed_team = &race_player_changed_team,
};
