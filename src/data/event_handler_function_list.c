/* Storage definition for event_handler_function_list (0x84174770, 428 bytes = 107 entries).
 * Reconstructed from the binary reloc order; each entry is a named UI event handler.
 * Element type from the DB applied type of the object itself:
 *   unsigned __int8 (*event_handler_function_list[107])(widget_instance *, event_record *, unsigned __int8 *)
 * DEVIATION: this file previously declared the element type `int (*)(...)`, taken from the
 * consumer externs; 60 of the handlers carried an auto2(MEDIUM) `int` return while 28 carried
 * uint8_t, for one uniform table. Disassembly agrees with the DB (90/105 return sites produce a
 * 0/1 or byte-width r3, none produce a wide value), so the whole family is uint8_t. */
#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"

typedef uint8_t (*event_handler_proc)(widget_instance *widget, event_record *event, uint8_t *widget_deleted);

extern uint8_t widget_event_function_null(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t widget_event_function_list_widget_goto_next_item(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t widget_event_function_list_widget_goto_previous_item(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t solo_level_initialize_list_single_player(widget_instance *list_widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t solo_level_initialize_list_coop(widget_instance *list_widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t solo_level_dispose_list(widget_instance *list_widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t solo_level_set_next_map_name(widget_instance *list_widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t difficulty_set(widget_instance *list_widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t start_new_game(widget_instance *list_widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t pause_game_restart_at_checkpoint(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t pause_game_restart_level(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t pause_game_quit_to_main_menu(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t clear_multiplayer_player_joins(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t player_wants_to_join_multiplayer_game(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t network_game_server_list_initialize(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t network_game_start_new_server(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t network_server_list_dispose(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t network_game_cancel(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t network_game_join_game_from_server_list(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t split_screen_game_initialize(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t coop_game_initialize(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t main_menu_initialize(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t multiplayer_type_menu_initialize(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t multiplayer_pick_quick_start_play_stage(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t multiplayer_level_list_initialize(widget_instance *list_widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t multiplayer_level_list_dispose(widget_instance *list_widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t multiplayer_level_select(widget_instance *wrapper_widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t multiplayer_profiles_list_initialize(widget_instance *list_widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t multiplayer_profiles_list_dispose(widget_instance *list_widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t multiplayer_profile_set_for_game(widget_instance *wrapper_widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t multiplayer_game_swap_teams(widget_instance *local_player_data_widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t netgame_join_player(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t player_profiles_list_initialize(widget_instance *list_widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t player_profiles_list_dispose(widget_instance *list_widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t player_profile_set_for_game_3wide(widget_instance *select_screen_widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t player_profile_set_for_game_1wide(widget_instance *list_widgets_parent, event_record *event, uint8_t *widget_deleted);
extern uint8_t playlist_profile_begin_editing(widget_instance *wrapper_widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t playlist_profile_end_editing(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t playlist_profile_set_game_engine(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t playlist_profile_change_name(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t playlist_profile_change_ctf_rules(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t playlist_profile_change_koth_rules(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t playlist_profile_change_slayer_rules(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t playlist_profile_change_oddball_rules(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t playlist_profile_change_racing_rules(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t playlist_profile_change_player_options(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t playlist_profile_change_item_options(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t playlist_profile_change_indicator_options(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t playlist_profile_initialize_game_engine(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t playlist_profile_initialize_name(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t playlist_profile_initialize_ctf_rules(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t playlist_profile_initialize_koth_rules(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t playlist_profile_initialize_slayer_rules(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t playlist_profile_initialize_oddball_rules(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t playlist_profile_initialize_racing_rules(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t playlist_profile_initialize_player_options(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t playlist_profile_initialize_item_options(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t playlist_profile_initialize_indicator_options(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t playlist_profile_save_changes(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t player_profile_color_picker_menu_initialize(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t player_profile_color_picker_menu_dispose(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t player_profile_color_picker_select_color(widget_instance *screen_widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t player_profile_begin_editing(widget_instance *wrapper_widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t player_profile_end_editing(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t player_profile_change_name(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t player_profile_save_changes(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t player_profile_initialize_controller_settings(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t player_profile_initialize_advanced_controller_settings(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t player_profile_change_controller_settings(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t player_profile_change_advanced_controller_settings(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t network_game_remove_local_player(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t switch_from_main_menu_to_single_player(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t delete_player_profile_request(widget_instance *wrapper_widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t delete_playlist_profile_request(widget_instance *wrapper_widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t delete_player_profile_final(widget_instance *wrapper_widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t delete_playlist_profile_final(widget_instance *wrapper_widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t cancel_profile_delete(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t create_and_begin_editing_new_gametype_profile(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t create_and_begin_editing_new_player_profile(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t network_game_start_faster(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t network_game_start_slower(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t network_game_server_accept_connections(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t network_game_server_defer_game_start(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t network_game_server_allow_game_start(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t disable_widget_if_no_xdemos(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t run_xdemos(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t single_player_reset_controller_choices(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t single_player_set_player1_controller_choice(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t single_player_set_player2_controller_choice(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t display_error_if_no_network_connection(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t start_network_game_if_no_advertised_servers(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t netgame_unjoin_player(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t close_calling_widget_if_not_editing_profile(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t exit_to_xbox_dashboard(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t new_campaign_chosen(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t new_campaign_decision(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t go_back_twice_next_time(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t difficulty_menu_initialize(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t begin_music_fade_out(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t new_campaign_if_no_custom_player_profiles_exist(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t exit_gracefully_to_xbox_dashboard(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t pause_game_invert_pitch(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t start_new_coop_game(widget_instance *list_widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t pause_game_invert_spinner_get_inverted(widget_instance *widget, event_record *event, uint8_t *widget_deleted);
extern uint8_t pause_game_invert_spinner_set_inverted(widget_instance *widget, event_record *event, uint8_t *widget_deleted);

uint8_t (*event_handler_function_list[107])(widget_instance *widget, event_record *event, uint8_t *widget_deleted) = {
    widget_event_function_null,
    widget_event_function_list_widget_goto_next_item,
    widget_event_function_list_widget_goto_previous_item,
    widget_event_function_null,
    widget_event_function_null,
    solo_level_initialize_list_single_player,
    solo_level_initialize_list_coop,
    solo_level_dispose_list,
    solo_level_set_next_map_name,
    difficulty_set,
    start_new_game,
    pause_game_restart_at_checkpoint,
    pause_game_restart_level,
    pause_game_quit_to_main_menu,
    clear_multiplayer_player_joins,
    player_wants_to_join_multiplayer_game,
    network_game_server_list_initialize,
    network_game_start_new_server,
    network_server_list_dispose,
    network_game_cancel,
    network_game_join_game_from_server_list,
    split_screen_game_initialize,
    coop_game_initialize,
    main_menu_initialize,
    multiplayer_type_menu_initialize,
    multiplayer_pick_quick_start_play_stage,
    multiplayer_level_list_initialize,
    multiplayer_level_list_dispose,
    multiplayer_level_select,
    multiplayer_profiles_list_initialize,
    multiplayer_profiles_list_dispose,
    multiplayer_profile_set_for_game,
    multiplayer_game_swap_teams,
    netgame_join_player,
    player_profiles_list_initialize,
    player_profiles_list_dispose,
    player_profile_set_for_game_3wide,
    player_profile_set_for_game_1wide,
    playlist_profile_begin_editing,
    playlist_profile_end_editing,
    playlist_profile_set_game_engine,
    playlist_profile_change_name,
    playlist_profile_change_ctf_rules,
    playlist_profile_change_koth_rules,
    playlist_profile_change_slayer_rules,
    playlist_profile_change_oddball_rules,
    playlist_profile_change_racing_rules,
    playlist_profile_change_player_options,
    playlist_profile_change_item_options,
    playlist_profile_change_indicator_options,
    playlist_profile_initialize_game_engine,
    playlist_profile_initialize_name,
    playlist_profile_initialize_ctf_rules,
    playlist_profile_initialize_koth_rules,
    playlist_profile_initialize_slayer_rules,
    playlist_profile_initialize_oddball_rules,
    playlist_profile_initialize_racing_rules,
    playlist_profile_initialize_player_options,
    playlist_profile_initialize_item_options,
    playlist_profile_initialize_indicator_options,
    playlist_profile_save_changes,
    player_profile_color_picker_menu_initialize,
    player_profile_color_picker_menu_dispose,
    player_profile_color_picker_select_color,
    player_profile_begin_editing,
    player_profile_end_editing,
    player_profile_change_name,
    player_profile_save_changes,
    player_profile_initialize_controller_settings,
    player_profile_initialize_advanced_controller_settings,
    player_profile_change_controller_settings,
    player_profile_change_advanced_controller_settings,
    network_game_remove_local_player,
    switch_from_main_menu_to_single_player,
    delete_player_profile_request,
    delete_playlist_profile_request,
    delete_player_profile_final,
    delete_playlist_profile_final,
    cancel_profile_delete,
    create_and_begin_editing_new_gametype_profile,
    create_and_begin_editing_new_player_profile,
    network_game_start_faster,
    network_game_start_slower,
    network_game_server_accept_connections,
    network_game_server_defer_game_start,
    network_game_server_allow_game_start,
    disable_widget_if_no_xdemos,
    run_xdemos,
    single_player_reset_controller_choices,
    single_player_set_player1_controller_choice,
    single_player_set_player2_controller_choice,
    display_error_if_no_network_connection,
    start_network_game_if_no_advertised_servers,
    netgame_unjoin_player,
    close_calling_widget_if_not_editing_profile,
    exit_to_xbox_dashboard,
    new_campaign_chosen,
    new_campaign_decision,
    go_back_twice_next_time,
    difficulty_menu_initialize,
    begin_music_fade_out,
    new_campaign_if_no_custom_player_profiles_exist,
    exit_gracefully_to_xbox_dashboard,
    pause_game_invert_pitch,
    start_new_coop_game,
    pause_game_invert_spinner_get_inverted,
    pause_game_invert_spinner_set_inverted,
};
