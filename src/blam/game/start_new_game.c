/* start_new_game @0x83691EA0 — widget callback: begin a new single-player campaign. If a second player was
 * requested (player_spawn_count > 1), find another connected gamepad besides the initiating controller to
 * be player 2; if none is found, silently drop back to 1 player and show a "no controller" error. Then
 * set up the fresh single-player session: difficulty normal, the first campaign map, standalone
 * connection, switch to the single-player menu flow, assign local player controllers, and remember
 * player 1's profile. Returns whether the game actually started. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/error_type.h"
#include "headers/game_difficulty.h"
#include "headers/game_connection.h"
#include "headers/blam_data_globals.h"


extern uint8_t input_has_gamepad(int16_t gamepad_index);
extern void display_error(int16_t error_code, int16_t local_player_index, uint8_t modal, uint8_t pause_game_time);
extern void main_set_difficulty(int16_t difficulty);
extern void main_set_map_name(const char *name);
extern void game_connection_set(int16_t new_connection);
extern void main_menu_switch_to_single_player(void);
extern void player_ui_set_single_player_local_player_controller(int16_t local_player_index, int16_t controller_index);
extern void player_ui_remember_player1_profile(uint8_t write_to_disk);

uint8_t start_new_game(widget_instance *list_widget, event_record *event, uint8_t *widget_deleted)
{
    int16_t second_player_controller = -1;
    int started = 1;

    if ( player_spawn_count > 1 )
    {
        int16_t controller = 0;
        while ( !input_has_gamepad(controller) || controller == event->controller_index )
        {
            if ( ++controller >= 2 )
                goto no_second_controller;
        }
        second_player_controller = controller;

no_second_controller:
        if ( second_player_controller == -1 )
        {
            player_spawn_count = 1;
            started = 0;
            display_error(_error_controller_coop_requires_two_controllers, -1, 1u, 0);
        }
    }

    if ( started )
    {
        main_set_difficulty(game_difficulty_level_normal);
        main_set_map_name(scenario_paths_0[0]);
        game_connection_set(_game_connection_local);
        main_menu_switch_to_single_player();
        player_ui_set_single_player_local_player_controller(0, event->controller_index);
        if ( second_player_controller != -1 )
            player_ui_set_single_player_local_player_controller(1, second_player_controller);
        player_ui_remember_player1_profile(0);
    }

    return started;
}
