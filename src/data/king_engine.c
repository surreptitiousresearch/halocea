/* king_engine @ 0x84186AF0 (.data, 156 bytes)
 * DB applied_types: game_engine king_engine;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x8211D7A0 -> "king"
 *   +0x04 type                       = 0x00000004
 *   +0x08 dispose                    = 0x8382BA28 -> king_engine_dispose
 *   +0x0C initialize                 = 0x8382D2E0 -> king_engine_initialize_for_new_map
 *   +0x10 dispose_from_old_map       = 0x8382BF00 -> king_engine_dispose_from_old_map
 *   +0x14 player_added               = 0x8382BF08 -> king_engine_player_added
 *   +0x18 player_removed             = 0x00000000
 *   +0x1C game_ending                = 0x8382BF90 -> king_engine_game_ending
 *   +0x20 game_starting              = 0x8382BF98 -> king_engine_game_starting
 *   +0x24 statistics_append          = 0x8382BFD8 -> king_engine_statistics_append
 *   +0x28 handle_client_message      = 0x8382BFE0 -> king_engine_handle_client_message
 *   +0x2C handle_server_message      = 0x8382BFE8 -> king_engine_handle_server_message
 *   +0x30 pregame_post_rasterize     = 0x8382BFF0 -> king_engine_pregame_post_rasterize
 *   +0x34 post_rasterize             = 0x8382D4A0 -> king_engine_post_rasterize
 *   +0x38 player_update              = 0x8382C0B8 -> king_engine_player_update
 *   +0x3C weapon_update              = 0x00000000
 *   +0x40 weapon_pickup              = 0x00000000
 *   +0x44 weapon_drop                = 0x00000000
 *   +0x48 update                     = 0x8382C590 -> king_engine_update
 *   +0x4C get_score                  = 0x8382C8F8 -> king_get_score
 *   +0x50 get_team_score             = 0x8382C938 -> king_get_team_score
 *   +0x54 get_score_string           = 0x8382CF30 -> king_get_score_string
 *   +0x58 get_score_header_string    = 0x8382CF88 -> king_get_score_header_string
 *   +0x5C get_team_score_string      = 0x8382CFF0 -> king_get_team_score_string
 *   +0x60 allow_pick_up              = 0x00000000
 *   +0x64 player_damaged_player      = 0x8382C710 -> king_engine_player_damaged_player
 *   +0x68 player_killed_player       = 0x8382C718 -> king_engine_player_killed_player
 *   +0x6C rasterize_score            = 0x8382C720 -> king_engine_display_score
 *   +0x70 starting_location_rating   = 0x00000000
 *   +0x74 prespawn_player_update     = 0x8382C8F0 -> king_engine_prespawn_player_update
 *   +0x78 postspawn_player_update    = 0x00000000
 *   +0x7C game_engine_player_get_team_index = 0x00000000
 *   +0x80 goal_matches_player        = 0x8382D038 -> king_engine_goal_matches_player
 *   +0x84 game_engine_test_flag      = 0x00000000
 *   +0x88 game_engine_test_trait     = 0x00000000
 *   +0x8C game_engine_did_player_win = 0x00000000
 *   +0x90 replicate_game_mode_state_to_network = 0x8382D058 -> king_engine_replicate_game_mode_state_to_network
 *   +0x94 replicate_game_mode_state_from_network = 0x8382D188 -> king_engine_replicate_game_mode_state_from_network
 *   +0x98 player_changed_team        = 0x8382D2A0 -> king_player_changed_team
 * second-order .data; game_engine reconstructed from binary bytes.
 */
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
extern uint8_t king_engine_initialize_for_new_map(void);
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
extern void king_engine_update(void); /* prototype not in DB */
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
