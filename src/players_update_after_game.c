/* players_update_after_game @ 0x836AE380 — the post-simulation player pass. For each player it:
 *   - runs the telefrag (spawn-camp kill) effect: while the telefrag timer counts up to 90 ticks the
 *     screen effect plays; at 90 the player's unit is killed (with a "you were killed" HUD message) unless
 *     the unit is flagged invulnerable;
 *   - counts down the two post-spawn invulnerability timers and clears the unit's invulnerable flag when
 *     the primary one expires;
 *   - tests the player's unit against the scenario's BSP-switch trigger volumes and, on a hit in the
 *     current BSP, schedules a structure-BSP switch;
 *   - examines nearby objects (client or server variant).
 * It then advances the recursive BSP-switch settle countdown, and maintains the map_lost / all-dead state
 * (rolling the malfunction-skull HUD glitch when enabled).
 *
 */

#include <wchar.h>
#include <stdint.h>
#include "headers/players_globals.h"
#include "headers/scenario.h"
#include "headers/scenario_bsp_switch_trigger_volume.h"
#include "headers/data_iterator.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/player_datum.h"
#include "headers/unit_datum.h"
#include "headers/game_connection.h"
#include "headers/object_damage_flags.h"
#include "headers/unit_flags.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/multiplayer_game_text_string.h"

extern const wchar_t empty_wide_string;

extern void game_set_players_are_double_speed(uint8_t players_are_double_speed);
extern void player_telefrag_effect_start(int player_index, float scale);
extern void player_telefrag_effect_stop(int player_index);
extern int tag_loaded(uint32_t group_tag, const char *name);
extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);
extern void hud_print_message(int16_t local_player_index, const wchar_t *text);
extern void unit_kill(int unit_index);
extern int object_get_ultimate_parent(int object_index);
extern uint8_t scenario_trigger_volume_test_object(int16_t trigger_volume_index, int object_index);
extern void main_switch_structure_bsp(int16_t new_structure_bsp_index);
extern int16_t game_connection(void);
extern void player_examine_nearby_objects(int player_index);
extern void player_examine_nearby_objects_client(int player_index);
extern uint8_t game_engine_running(void);
extern void main_lost_map(void);
extern uint32_t *get_global_local_random_seed_address(void);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);

/* object data pointer for an object handle (object header datum: data ptr at int element 3*idx+2) */
static unit_datum *object_data_ptr(int object_handle)
{
    return ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_handle)->datum);
}

void players_update_after_game(void)
{
    int16_t double_speed_duration = players_globals->players_are_double_speed_duration;
    data_iterator iter;
    player_datum *player;
    int result = 0;   /* internal scratch only; caller (game_tick) ignores r3 -> void */

    if ( double_speed_duration > 0 )
    {
        players_globals->players_are_double_speed_duration = double_speed_duration - 1;
        if ( double_speed_duration == 1 )
            game_set_players_are_double_speed(0);
    }

    data_iterator_new(&iter, player_data);
    for ( player = data_iterator_next(&iter); player; player = data_iterator_next(&iter) )
    {
        int telefrag_timer = player->telefrag_timeout;
        int unit_index;

        /* telefrag effect / kill */
        if ( player->is_blocking_teleporter )
        {
            if ( telefrag_timer < 90 )
            {
                player_telefrag_effect_start(iter.index, ((float)telefrag_timer * 0.011111111f));
            }
            else if ( player->unit_index != -1
                   && (object_data_ptr(player->unit_index)->object.damage_flags & (1u << _object_die_act_of_god_bit)) == 0 )
            {
                if ( (uint16_t)player->local_player_index != 0xFFFF )
                {
                    int text_tag = tag_loaded(0x75737472u /* 'ustr' */, "ui\\multiplayer_game_text");
                    const wchar_t *text = text_tag == -1 ? &empty_wide_string
                                                         : unicode_string_list_get_string(text_tag, _string_you_were_telefragged);
                    hud_print_message(player->local_player_index, text);
                }
                player_telefrag_effect_stop(iter.index);
                unit_kill(player->unit_index);
            }
        }
        else if ( telefrag_timer > 0 )
        {
            player->telefrag_timeout = telefrag_timer - 1;
        }

        player->is_blocking_teleporter = 0;

        /* powerup / post-spawn invulnerability timers */
        unit_index = player->unit_index;
        if ( unit_index != -1 )
        {
            player_datum *player_base = DATA_ARRAY_ELEMENT(player_data, player_datum, iter.index);
            int i;
            for ( i = 0; i < 2; i++ )
            {
                int16_t *timer = &player_base->powerup_durations[i];
                if ( *timer > 0 )
                {
                    int16_t remaining = *timer;
                    *timer = remaining - 1;
                    if ( remaining == 1 && !i )
                    {
                        unit_datum *unit_data = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum,
                            DATA_ARRAY_ELEMENT(player_data, player_datum, (int16_t)iter.index)->unit_index)->datum;
                        unit_data->unit.flags &= ~(1u << _unit_active_camouflaged_bit);   /* clear post-spawn invuln flag */
                    }
                }
            }
        }

        /* BSP-switch trigger volumes */
        if ( player->unit_index != -1 )
        {
            int ultimate_parent = object_get_ultimate_parent(player->unit_index);
            if ( (object_data_ptr(ultimate_parent)->object.flags & (1u << _object_outside_of_map_bit)) == 0 )
            {
                int i;
                for ( i = 0; i < global_scenario->bsp_switch_trigger_volumes.count; i++ )
                {
                    scenario_bsp_switch_trigger_volume *trigger =
                        &((scenario_bsp_switch_trigger_volume *)global_scenario->bsp_switch_trigger_volumes.address)[i];
                    if ( trigger->source_bsp_index == global_structure_bsp_index
                      && scenario_trigger_volume_test_object(trigger->trigger_volume_index, player->unit_index) )
                    {
                        /* recovered: bsp_switch_state nibble math -> 0x1F bitfields */
                        players_globals->bsp_check_recursive_switch_ticks = 0;
                        players_globals->local_player_triggered_switch = player->local_player_index;
                        players_globals->bsp_switch_trigger_index = (int16_t)i;
                        main_switch_structure_bsp(trigger->destination_bsp_index);
                    }
                }
            }
        }

        /* per-frame scratch reset + nearby-object scan */
        {
            player_datum *player_base = DATA_ARRAY_ELEMENT(player_data, player_datum, iter.index);
            player_base->action_result = 0;
            player_base->action_object_index = -1;
        }
        if ( game_connection() == _game_connection_network_client )
            player_examine_nearby_objects_client(iter.index);
        else
            player_examine_nearby_objects(iter.index);
    }

    /* recursive BSP-switch settle countdown (low nibble of +0x1F); saturates and resets past 0xC */
    {
        /* recovered: bsp_switch_state nibble math -> 0x1F bitfields (triggered==-1 means idle) */
        if ( players_globals->local_player_triggered_switch != -1 )
        {
            unsigned int new_ticks = (players_globals->bsp_check_recursive_switch_ticks + 1) & 0xF;
            players_globals->bsp_check_recursive_switch_ticks = new_ticks;
            if ( new_ticks > 0xC )
            {
                players_globals->local_player_triggered_switch = -1;
                players_globals->bsp_check_recursive_switch_ticks = 0;
            }
        }
    }

    /* map-lost / all-dead state machine */
    if ( players_globals->all_dead )
    {
        result = game_engine_running();
        if ( !(unsigned char)result && !map_lost )
        {
            main_lost_map();
            if ( hcex_malfunction_skull )
            {
                unsigned int *seed = get_global_local_random_seed_address();
                result = seed_random_range(seed, 0, 6);
                hcex_off_hud_element = (int16_t)result % 6;
                map_lost = 1;
            }
            else
            {
                map_lost = 1;
                hcex_off_hud_element = -1;
            }
        }
    }
    else
    {
        map_lost &= (map_lost != 0) - 1;   /* clear */
    }
}
