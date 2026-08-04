/* ctf_engine @ 0x84185438 -- second-order .data; game_engine reconstructed from binary bytes. */
#include <stdint.h>
#include "../headers/game_engine.h"

#include "headers/scenario_player.h"
#include "headers/game_statistics.h"
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/weapon_datum.h"
extern uint8_t ctf_engine_allow_pick_up(int unit_index, int item_index);
extern uint8_t ctf_engine_display_score(int player_index, int message, int message_data, uint16_t *buffer, int buffer_size);
extern void ctf_engine_dispose(void); /* prototype not in DB */
extern void ctf_engine_dispose_from_old_map(void); /* prototype not in DB */
extern void ctf_engine_game_ending(void); /* prototype not in DB */
extern void ctf_engine_game_starting(void); /* prototype not in DB */
extern void ctf_engine_handle_client_message(int player_index, void *encoded_message, int16_t encoded_message_size);
extern void ctf_engine_handle_server_message(void *encoded_message, int16_t encoded_message_size);
extern unsigned __int8 ctf_engine_initialize_for_new_map(void);
extern void ctf_engine_player_added(int player_index);
extern void ctf_engine_player_damaged_player(int killing_player_index, int dead_player_index, uint8_t friendly_fire);
extern void ctf_engine_player_killed_player(int killing_player_index, int killing_object_index, int dead_player_index, uint8_t friendly_fire);
extern void ctf_engine_player_update(int player_index);
extern void ctf_engine_post_rasterize(void); /* prototype not in DB */
extern void ctf_engine_pregame_post_rasterize(void); /* prototype not in DB */
extern void ctf_engine_prespawn_player_update(int player_index);
extern void ctf_engine_replicate_game_mode_state_from_network(message_delta_processor_header *header, network_game_client *client);
extern void ctf_engine_replicate_game_mode_state_to_network(message_delta_processor_mode mode, const int machine_index);
extern float ctf_engine_starting_location_rating(int player_index, scenario_player *starting_location);
extern void ctf_engine_statistics_append(game_statistics *permanent_statistics, game_statistics *game_statistics);
extern void ctf_engine_update(void); /* prototype not in DB */
extern void ctf_engine_weapon_update(int weapon_index, weapon_datum *weapon_ptr);
extern int ctf_get_score(int player_index, get_score_type get_score_type);
extern uint16_t * ctf_get_score_header_string(uint16_t *buffer);
extern uint16_t * ctf_get_score_string(int player_index, uint16_t *buffer);
extern int ctf_get_team_score(int team_index);
extern uint16_t * ctf_get_team_score_string(int team_index, uint16_t *buffer);
extern void ctf_player_changed_team(int player_index, uint8_t new_team);
extern uint8_t ctf_test_flag(int flag);
extern void ctf_weapon_drop(int weapon_index);
extern uint8_t ctf_weapon_pickup(int weapon_index, int player_index);

struct game_engine ctf_engine = {
    .name = "ctf",
    .type = 1,
    .dispose = &ctf_engine_dispose,
    .initialize = &ctf_engine_initialize_for_new_map,
    .dispose_from_old_map = &ctf_engine_dispose_from_old_map,
    .player_added = &ctf_engine_player_added,
    .game_ending = &ctf_engine_game_ending,
    .game_starting = &ctf_engine_game_starting,
    .statistics_append = &ctf_engine_statistics_append,
    .handle_client_message = &ctf_engine_handle_client_message,
    .handle_server_message = &ctf_engine_handle_server_message,
    .pregame_post_rasterize = &ctf_engine_pregame_post_rasterize,
    .post_rasterize = &ctf_engine_post_rasterize,
    .player_update = &ctf_engine_player_update,
    .weapon_update = &ctf_engine_weapon_update,
    .weapon_pickup = &ctf_weapon_pickup,
    .weapon_drop = &ctf_weapon_drop,
    .update = &ctf_engine_update,
    .get_score = &ctf_get_score,
    .get_team_score = &ctf_get_team_score,
    .get_score_string = &ctf_get_score_string,
    .get_score_header_string = &ctf_get_score_header_string,
    .get_team_score_string = &ctf_get_team_score_string,
    .allow_pick_up = &ctf_engine_allow_pick_up,
    .player_damaged_player = &ctf_engine_player_damaged_player,
    .player_killed_player = &ctf_engine_player_killed_player,
    .rasterize_score = &ctf_engine_display_score,
    .starting_location_rating = &ctf_engine_starting_location_rating,
    .prespawn_player_update = &ctf_engine_prespawn_player_update,
    .game_engine_test_flag = &ctf_test_flag,
    .replicate_game_mode_state_to_network = &ctf_engine_replicate_game_mode_state_to_network,
    .replicate_game_mode_state_from_network = &ctf_engine_replicate_game_mode_state_from_network,
    .player_changed_team = &ctf_player_changed_team,
};
