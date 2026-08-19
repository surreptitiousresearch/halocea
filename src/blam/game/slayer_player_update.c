/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* slayer_player_update @0x83815D20 — per-player per-tick update shared by the slayer-family engines (the
 * game_engine_variant union is read through its ctf view, which is how the flag-carry fields alias here). When
 * "reset on capture" is set it bleeds the carry-progress value (+108) down toward 1.0; when "assault" is set it
 * ramps it up toward 1.0. When "flag must reset" is set it clears the player's goal marker and, if the player is
 * carrying a target object, re-points the goal at that object's origin (+160); on a client it also finds a new
 * target when the current one is invalid or manned-out. Finally, if the player's team score reaches the
 * variant's score-to-win, the game ends.
 *
 * Player record fields by raw offset: +108 carry progress, +32 team index, +136 carried target player, +52
 * (word 13) held object. Object origin via the salted object-header stride. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/game_variant.h"
#include "headers/slayer_globals.h"
#include "headers/real_point3d.h"
#include "headers/player_datum.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/game_connection.h"
#include "headers/blam_data_globals.h"


#include "headers/real_point3d.h"
extern game_variant *game_engine_get_variant(void);
extern void game_engine_clear_goal_position(int16_t index);
extern void game_engine_set_goal_position(int16_t index, real_point3d *position, float vertical_offset, char *descriptor, int player_index, int16_t team_index, int ignore_player_index);
extern int16_t game_connection(void);
extern void find_next_target(int my_index);
extern uint8_t game_engine_man_out(int player_index);
extern void game_engine_end_game(void);

void slayer_player_update(int player_index)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    if ( game_engine_get_variant()->game_engine_variant.ctf.reset_on_capture && player->multiplayer.speed_multiplier > 1.0f )
    {
        float carry = player->multiplayer.speed_multiplier - 0.00011111111f;
        player->multiplayer.speed_multiplier = carry;
        if ( carry <= 1.0f )
            carry = 1.0f;
        player->multiplayer.speed_multiplier = carry;
    }

    if ( game_engine_get_variant()->game_engine_variant.ctf.assault && player->multiplayer.speed_multiplier < 1.0f )
    {
        float carry = player->multiplayer.speed_multiplier + 0.000011111111f;
        player->multiplayer.speed_multiplier = carry;
        if ( carry > 1.0f )
            carry = 1.0f;
        player->multiplayer.speed_multiplier = carry;
    }

    if ( game_engine_get_variant()->game_engine_variant.ctf.flag_must_reset )
    {
        game_engine_clear_goal_position(player_index);

        int carried_target_player = player->multiplayer.special;
        if ( carried_target_player != -1 )
        {
            player_datum *target_player = DATA_ARRAY_ELEMENT(player_data, player_datum, carried_target_player);
            int object_index = target_player->unit_index;
            if ( object_index != -1 )
            {
                object_header_datum *object_entry =
                    DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index);
                /* Disasm-verified register loads (0x83815E00-0x83815E54): r3=index(player_index),
                 * r4=position, f1=vertical_offset(0), r6=descriptor("target_blue") with r5 the FP shadow
                 * slot, r7=player_index, r8=team_index(-1), r9=ignore_player_index(-1). The prior extern's
                 * spurious `object` arg mapped to the r5 shadow; object_entry is only used to derive the
                 * position pointer. */
                game_engine_set_goal_position(player_index, &object_entry->datum->object.bounding_sphere_center,
                    0.0f, "target_blue", player_index, -1, -1);
            }
        }

        if ( game_connection() == _game_connection_network_server )
        {
            if ( player->unit_index != -1 && player->multiplayer.special == -1 )
                find_next_target(player_index);
            int target = player->multiplayer.special;
            if ( target != -1 && game_engine_man_out(target) )
                find_next_target(player_index);
        }
    }

    int team_score = slayer_globals.team_score[player->team_index];
    if ( team_score >= game_engine_get_variant()->universal_variant.score_to_win )
        game_engine_end_game();
}
