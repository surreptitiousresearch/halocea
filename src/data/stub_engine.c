/* stub_engine @ 0x841868B8 (.data, 156 bytes)
 * DB applied_types: game_engine stub_engine;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 name                       = 0x820F2FA8 -> "stub"
 *   +0x04 type                       = 0x00000007
 *   +0x08 dispose                    = 0x83829CE0 -> stub_engine_dispose
 *   +0x0C initialize                 = 0x83829CE8 -> stub_engine_initialize_for_new_map
 *   +0x10 dispose_from_old_map       = 0x83829CF0 -> stub_engine_dispose_from_old_map
 *   +0x14 player_added               = 0x83829CF8 -> stub_engine_player_added
 *   +0x18 player_removed             = 0x00000000
 *   +0x1C game_ending                = 0x83829D00 -> stub_engine_game_ending
 *   +0x20 game_starting              = 0x83829D08 -> stub_engine_game_starting
 *   +0x24 statistics_append          = 0x83829D10 -> stub_engine_statistics_append
 *   +0x28 handle_client_message      = 0x83829D18 -> stub_engine_handle_client_message
 *   +0x2C handle_server_message      = 0x83829D20 -> stub_engine_handle_server_message
 *   +0x30 pregame_post_rasterize     = 0x83829D28 -> stub_engine_pregame_post_rasterize
 *   +0x34 post_rasterize             = 0x83829D30 -> stub_engine_post_rasterize
 *   +0x38 player_update              = 0x00000000
 *   +0x3C weapon_update              = 0x00000000
 *   +0x40 weapon_pickup              = 0x00000000
 *   +0x44 weapon_drop                = 0x00000000
 *   +0x48 update                     = 0x83829D38 -> stub_engine_update
 *   +0x4C get_score                  = 0x00000000
 *   +0x50 get_team_score             = 0x00000000
 *   +0x54 get_score_string           = 0x00000000
 *   +0x58 get_score_header_string    = 0x00000000
 *   +0x5C get_team_score_string      = 0x00000000
 *   +0x60 allow_pick_up              = 0x83829D40 -> stub_engine_allow_pick_up
 *   +0x64 player_damaged_player      = 0x83829D48 -> stub_engine_player_damaged_player
 *   +0x68 player_killed_player       = 0x83829D50 -> stub_engine_player_killed_player
 *   +0x6C rasterize_score            = 0x00000000
 *   +0x70 starting_location_rating   = 0x00000000
 *   +0x74 prespawn_player_update     = 0x00000000
 *   +0x78 postspawn_player_update    = 0x00000000
 *   +0x7C game_engine_player_get_team_index = 0x00000000
 *   +0x80 goal_matches_player        = 0x00000000
 *   +0x84 game_engine_test_flag      = 0x00000000
 *   +0x88 game_engine_test_trait     = 0x00000000
 *   +0x8C game_engine_did_player_win = 0x00000000
 *   +0x90 replicate_game_mode_state_to_network = 0x00000000
 *   +0x94 replicate_game_mode_state_from_network = 0x00000000
 *   +0x98 player_changed_team        = 0x00000000
 * second-order .data; game_engine reconstructed from binary bytes.
 */
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
