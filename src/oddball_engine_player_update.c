/* oddball_engine_player_update @0x83817288 — per-frame Oddball update for one player. Refreshes ball ownership,
 * counts how many balls this player is holding, and sets the player's movement scale (base 1.0, then 1.0 / 1.25 /
 * 0.75 by the "speed with ball" setting when carrying). Carrying a ball strips active camo unless the ball trait
 * is invisibility. While scoring is allowed and the ball type is not the terminator variant, the player accrues
 * one score per held ball (announcing "you have the ball" on a normal ball). A non-normal, in-range ball type
 * fires the ball-status message. Finally, if the player's unit is holding a flag-class weapon, the player's
 * individual score /30 is written onto that flag object (with a periodic scoring cue every 150 points).
 *
 * DEVIATION: Hex-Rays rendered the "ball type != terminator" gate as a convoluted byte subtraction
 * (`X-2 != X-3 + (type==terminator)`); disasm (0x83817380-0x83817390) shows the addic/subfe "!= 0" idiom applied
 * to oddball_ball_type - 2, i.e. simply oddball_ball_type != _oddball_terminator. The v18/v21 pointer dance
 * resolves to (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, flag_object_index)->datum) (the slot-base plus the +8 load is the object pointer). */

#include <stdint.h>
#include "headers/game_variant.h"
#include "headers/oddball_globals.h"
#include "headers/oddball_ball_type.h"
#include "headers/game_engine_trait.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/weapon_datum.h"
#include "headers/player_datum.h"
#include "headers/oddball_message.h"
#include "headers/game_engine_multiplayer_sound.h"
#include "headers/blam_data_globals.h"


extern void game_engine_state_message(int player_index, int message, int message_data);
extern void update_ball_ownership(int player_index);
extern game_variant *game_engine_get_variant(void);
extern uint8_t game_engine_can_score(void);
extern void game_engine_player_depower_active_camo(int player_index);
extern void oddball_add_score(int player_index);
extern int16_t game_connection(void);
extern uint8_t weapon_is_flag(int weapon_index);
extern void game_engine_play_multiplayer_sound(int index, uint8_t should_replicate);


void oddball_engine_player_update(int player_index)
{
    uint16_t player_slot = (uint16_t)player_index;
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    game_engine_state_message(player_index, -1, -1);
    update_ball_ownership(player_index);

    int owned_balls = 0;
    game_variant *variant = game_engine_get_variant();
    int ball_spawn_count = variant->game_engine_variant.oddball.ball_spawn_count;
    if ( ball_spawn_count > 0 )
    {
        for ( int i = 0; i < ball_spawn_count; ++i )
            if ( oddball_globals.current_ball_owner[i] == player_index )
                ++owned_balls;
    }

    player->multiplayer.speed_multiplier = 1.0f;
    if ( owned_balls > 0 )
    {
        if ( game_engine_get_variant()->game_engine_variant.oddball.trait_with_ball != _game_trait_invisible )
            game_engine_player_depower_active_camo(player_index);
        int speed_setting = game_engine_get_variant()->game_engine_variant.ctf.single_flag_time;
        if ( speed_setting == 1 )
            player->multiplayer.speed_multiplier = 1.0f;
        else if ( speed_setting == 2 )
            player->multiplayer.speed_multiplier = 1.25f;
        else
            player->multiplayer.speed_multiplier = 0.75f;
    }

    if ( game_engine_can_score() )
    {
        if ( game_engine_get_variant()->game_engine_variant.oddball.oddball_ball_type != _oddball_terminator
                && owned_balls > 0 )
        {
            for ( int n = owned_balls; n > 0; --n )
            {
                if ( game_engine_get_variant()->game_engine_variant.oddball.oddball_ball_type == _oddball_normal )
                    game_engine_state_message(player_index, oddball_message_you_have_the_ball_tick, player_index);
                game_connection();
                oddball_add_score(player_index);
            }
        }
    }

    oddball_ball_type ball_type = game_engine_get_variant()->game_engine_variant.oddball.oddball_ball_type;
    if ( ball_type > _oddball_normal && ball_type <= _oddball_terminator && owned_balls > 0 )
        game_engine_state_message(player_index, oddball_message_you_are_it, player_index);

    if ( player->unit_index != -1 )
    {
        unit_datum *unit = (unit_datum *)(DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, player->unit_index)->datum);
        int16_t weapon_slot = unit->unit.current_weapon_index;
        if ( weapon_slot != -1 )
        {
            int flag_object_index = unit->unit.weapon_object_indices[weapon_slot];
            if ( flag_object_index != -1 && weapon_is_flag(flag_object_index) )
            {
                weapon_datum *flag_object = (weapon_datum *)(DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, flag_object_index)->datum);
                int score = oddball_globals.individual_score[player_slot];
                if ( score > 0 && !(score % 150) && score < oddball_globals.score_to_win )
                    game_engine_play_multiplayer_sound(_multiplayer_sound_countdown_timer_end, 0);
                flag_object->weapon.magazines[0].rounds_loaded = (uint16_t)(score / 30);
            }
        }
    }
}
