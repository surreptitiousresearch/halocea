/* ctf_engine_weapon_update @0x838075C8 — per-frame Capture-the-Flag maintenance for one flag weapon object
 * (sibling of ctf_engine_player_update). On the host it does three things:
 *   1. Single-flag CTF swap: while the flag-swap timer counts down, when it expires and the flag is not
 *      currently held, announce the swap, clear both warnings, delete this flag, spawn the opposing team's
 *      flag, play the swap sound, reset it, clear the goal markers, and restart the swap timer.
 *   2. Idle-flag return: if a dropped flag has sat unowned for more than 0x1FE ticks (and is flagged
 *      "at home"), play the return sound / score banners (when the weapon's auto-return flag 0x40 is set)
 *      and reset the flag.
 *   3. Goal markers: locate the player carrying this flag (if any) and, when scoring applies to this team,
 *      set the flag's carried-position goal ("flag_blue") plus, if a carrier exists, a secondary "default"
 *      goal at the flag's home position raised by 0.5.
 *
 * The weapon_datum's object/item/weapon sub-fields are resolved to the DB weapon_datum struct
 * (owner_team_index @0xB8, object.flags @0x10, parent_object_index @0x11C, item.flags @0x1F4,
 * item.last_owned_time @0x204, weapon.flags @0x22C), matching ctf_engine_player_update.
 *
 * DEVIATIONS: (1) both game_engine_set_goal_position calls are FPR-shadow-scrambled by their float
 * vertical_offset (0.0) burning a GPR slot — remapped to the verified 7-arg signature (index, position,
 * vertical_offset, descriptor, player_index, team_index, ignore_player_index) from the call-site disasm at
 * 0x838078D8/0x8380790C. (2) the flag-return sound index `(cntlzw(team) & 0x20) == 0) + 37` is the
 * branchless `37 + (owner_team_index != _multiplayer_team_red)`. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/item_flags.h"
#include "headers/object_header_datum.h"
#include "headers/data_iterator.h"
#include "headers/ctf_globals.h"
#include "headers/game_variant.h"
#include "headers/scenario_netgame_flag.h"
#include "headers/real_point3d.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_datum_flags.h"
#include "headers/player_datum.h"
#include "headers/game_connection.h"
#include "headers/multiplayer_team.h"
#include "headers/object_flags.h"
#include "headers/game_engine_multiplayer_sound.h"
#include "headers/ctf_message.h"
#include "headers/blam_data_globals.h"


extern int16_t game_connection(void);
extern game_variant *game_engine_get_variant(void);
extern int game_time_get(void);
extern void game_show_score(int recipient_player_index, int subject_player_index, int message, uint8_t should_replicate);
extern void game_show_score_team(int team, int message, uint8_t should_replicate);
extern void game_engine_clear_goal_position(int16_t index);
extern void game_engine_set_goal_position(int16_t index, real_point3d *position, float vertical_offset, char *descriptor, int player_index, int16_t team_index, int ignore_player_index);
extern void game_engine_play_multiplayer_sound(int index, uint8_t should_replicate);
extern void object_delete(int object_index);
extern void create_the_flag(int team_index);
extern void weapon_reset_flag(int weapon_index);
extern void ctf_single_flag_what_is_up_message(int flag_team_index);
extern uint8_t weapon_is_flag(int weapon_index);
extern unsigned __int8 item_get_position_even_if_in_inventory(int item_index, real_point3d *position);
extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);
extern uint8_t unit_has_weapon(int unit_index, int weapon_index);


void ctf_engine_weapon_update(int weapon_index, weapon_datum *weapon_ptr)
{
    weapon_datum *weapon = weapon_ptr;

    /* 1. Single-flag swap timer. */
    if ( game_connection() == _game_connection_network_server
      && game_engine_get_variant()->game_engine_variant.ctf.single_flag_time > 0 )
    {
        int previous_timer = ctf_globals.flag_swap_timer;
        unsigned __int8 expired = ctf_globals.flag_swap_timer == 0;
        if ( ctf_globals.flag_swap_timer > 0 )
        {
            --ctf_globals.flag_swap_timer;
            expired = previous_timer == 1;
        }

        if ( expired && (weapon->item.flags & (1u << _item_in_unit_inventory_bit)) == 0 )
        {
            int owner_team_index = weapon->object.owner_team_index;

            game_show_score(-1, -1, ctf_message_time_expired, 1u);
            ctf_globals.flag_warning[0] = 0;
            ctf_globals.flag_warning_time_in_ticks[0] = 0;
            ctf_globals.flag_warning[1] = 0;
            ctf_globals.flag_warning_time_in_ticks[1] = 0;
            ctf_globals.weapon_index[owner_team_index] = -1;
            game_engine_clear_goal_position(0);
            game_engine_clear_goal_position(1);

            int new_team = (owner_team_index + 1) % 2;
            object_delete(weapon_index);
            create_the_flag(new_team);
            ctf_globals.team_with_flag = new_team;

            weapon_index = ctf_globals.weapon_index[new_team];
            weapon = (weapon_datum *)(DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum);

            game_engine_play_multiplayer_sound(_multiplayer_sound_blue_team_ctf
                    + (weapon->object.owner_team_index != _multiplayer_team_red), 1u);
            weapon_reset_flag(weapon_index);
            game_engine_clear_goal_position(2);
            game_engine_clear_goal_position(3);
            ctf_globals.flag_swap_timer =
                    game_engine_get_variant()->game_engine_variant.ctf.single_flag_time;
            ctf_single_flag_what_is_up_message(weapon->object.owner_team_index);
        }
    }

    /* 2. Idle-flag auto-return. */
    if ( game_connection() == _game_connection_network_server
      && (unsigned int)(game_time_get() - weapon->item.last_owned_time) > 0x1FE   /* item.last_owned_time */
      && weapon_is_flag(weapon_index)
      && ((weapon->object.flags >> _object_connected_to_map_bit) & 1) != 0  /* at home */
      && weapon->object.parent_object_index == -1 )                         /* object.parent_object_index */
    {
        int owner_team_index = weapon->object.owner_team_index;
        if ( (weapon->weapon.flags & (1u << _weapon_multiplayer_flag)) != 0 ) /* auto-return home */
        {
            game_engine_play_multiplayer_sound(owner_team_index == _multiplayer_team_red
                    ? _multiplayer_sound_ctf_red_returned_flag
                    : _multiplayer_sound_ctf_blue_returned_flag, 1u);
            __int16 team = weapon->object.owner_team_index;
            ctf_globals.flag_warning[team] = 0;
            ctf_globals.flag_warning_time_in_ticks[team] = 0;
            game_show_score_team(owner_team_index, ctf_message_your_flag_was_returned, 1u);
            game_show_score_team((owner_team_index + 1) % 2, ctf_message_enemy_flag_was_returned, 1u);
        }
        weapon_reset_flag(weapon_index);
    }

    /* 3. Goal markers. Find the player carrying this flag (if any). */
    int carrier_player_index = -1;
    data_iterator iterator;
    data_iterator_new(&iterator, player_data);
    for ( player_datum *player = data_iterator_next(&iterator); player;
          player = data_iterator_next(&iterator) )
    {
        int unit_index = player->unit_index;   /* player.unit_index @0x34 */
        if ( unit_index != -1 && unit_has_weapon(unit_index, weapon_index) )
        {
            carrier_player_index = iterator.index;
            break;
        }
    }

    int owner_team_index = weapon->object.owner_team_index;
    __int16 other_team_index = (owner_team_index + 1) % 2;

    real_point3d flag_position;
    unsigned __int8 show_goal = item_get_position_even_if_in_inventory(weapon_index, &flag_position);
    if ( game_engine_get_variant()->game_engine_variant.ctf.single_flag_time > 0 )
        show_goal &= (owner_team_index != ctf_globals.team_with_flag) - 1;

    if ( show_goal == 1 )
    {
        game_engine_set_goal_position(owner_team_index, &flag_position, 0.0f, "flag_blue", -1,
                other_team_index, carrier_player_index);

        if ( carrier_player_index == -1 )
        {
            game_engine_clear_goal_position(owner_team_index + 2);
        }
        else
        {
            scenario_netgame_flag *home_flag = ctf_globals.flags[other_team_index];
            flag_position.n[0] = home_flag->position.n[0];
            flag_position.n[1] = home_flag->position.n[1] + 0.5f;
            flag_position.n[2] = home_flag->position.n[2];
            game_engine_set_goal_position(owner_team_index + 2, &flag_position, 0.0f, "default",
                    carrier_player_index, -1, -1);
        }
    }
}
