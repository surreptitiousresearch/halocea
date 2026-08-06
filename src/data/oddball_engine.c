/* oddball_engine @ 0x84185940 (.data, 156 bytes)
 * DB applied_types: game_engine oddball_engine;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x8211D7A8 -> "oddball"
 *   +0x04 type                       = 0x00000003
 *   +0x08 dispose                    = 0x83816030 -> oddball_engine_dispose
 *   +0x0C initialize                 = 0x83816FE0 -> oddball_engine_initialize_for_new_map
 *   +0x10 dispose_from_old_map       = 0x83816038 -> oddball_engine_dispose_from_old_map
 *   +0x14 player_added               = 0x83816040 -> oddball_engine_player_added
 *   +0x18 player_removed             = 0x00000000
 *   +0x1C game_ending                = 0x838160B8 -> oddball_engine_game_ending
 *   +0x20 game_starting              = 0x838160C0 -> oddball_engine_game_starting
 *   +0x24 statistics_append          = 0x838160C8 -> oddball_engine_statistics_append
 *   +0x28 handle_client_message      = 0x838160D0 -> oddball_engine_handle_client_message
 *   +0x2C handle_server_message      = 0x838160D8 -> oddball_engine_handle_server_message
 *   +0x30 pregame_post_rasterize     = 0x838160E0 -> oddball_engine_pregame_post_rasterize
 *   +0x34 post_rasterize             = 0x838160E8 -> oddball_engine_post_rasterize
 *   +0x38 player_update              = 0x83817288 -> oddball_engine_player_update
 *   +0x3C weapon_update              = 0x83817510 -> oddball_engine_weapon_update
 *   +0x40 weapon_pickup              = 0x83817AF0 -> oddball_weapon_pickup
 *   +0x44 weapon_drop                = 0x83816890 -> oddball_weapon_drop
 *   +0x48 update                     = 0x83817738 -> oddball_engine_update
 *   +0x4C get_score                  = 0x83816898 -> oddball_get_score
 *   +0x50 get_team_score             = 0x838168E8 -> oddball_get_team_score
 *   +0x54 get_score_string           = 0x83816A88 -> oddball_get_score_string
 *   +0x58 get_score_header_string    = 0x83816B18 -> oddball_get_score_header_string
 *   +0x5C get_team_score_string      = 0x83816BB0 -> oddball_get_team_score_string
 *   +0x60 allow_pick_up              = 0x00000000
 *   +0x64 player_damaged_player      = 0x838163B0 -> oddball_engine_player_damaged_player
 *   +0x68 player_killed_player       = 0x838178C0 -> oddball_engine_player_killed_player
 *   +0x6C rasterize_score            = 0x838164B8 -> oddball_engine_display_score
 *   +0x70 starting_location_rating   = 0x00000000
 *   +0x74 prespawn_player_update     = 0x83816888 -> oddball_engine_prespawn_player_update
 *   +0x78 postspawn_player_update    = 0x00000000
 *   +0x7C game_engine_player_get_team_index = 0x00000000
 *   +0x80 goal_matches_player        = 0x00000000
 *   +0x84 game_engine_test_flag      = 0x838169A0 -> oddball_test_flag
 *   +0x88 game_engine_test_trait     = 0x838169F0 -> oddball_test_trait
 *   +0x8C game_engine_did_player_win = 0x00000000
 *   +0x90 replicate_game_mode_state_to_network = 0x83816C40 -> oddball_engine_replicate_game_mode_state_to_network
 *   +0x94 replicate_game_mode_state_from_network = 0x83816DB0 -> oddball_engine_replicate_game_mode_state_from_network
 *   +0x98 player_changed_team        = 0x83816ED8 -> oddball_player_changed_team
 * second-order .data; game_engine reconstructed from binary bytes.
 */
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
