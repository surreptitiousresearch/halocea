/* Blam .lib data-global storage definition (init batch 7). */
#include "../headers/widget_instance.h"
/* game_data_input_function_list @0x84181548, DB extent 172 = 43 function pointers (0x2B).
 * Reconstructed from binary relocation order. */
extern void widget_function_null(widget_instance *widget);
extern void settings_menu_update_extended_description(widget_instance *list_widget);
extern void playlist_settings_menu_update_extended_description(widget_instance *list_widget);
extern void playlist_gametype_select_menu_update_extended_description(widget_instance *list_widget);
extern void multiplayer_type_menu_update_extended_description(widget_instance *list_widget);
extern void solo_level_select_list_update_displayed_items(widget_instance *list_widget);
extern void difficulty_select_menu_update_extended_description(widget_instance *list_widget);
extern void set_textbox_to_build_number(widget_instance *widget);
extern void server_list_menu_update(widget_instance *widget);
extern void network_pregame_status_screen_update(widget_instance *widget);
extern void splitscreen_pregame_status_screen_update(widget_instance *widget);
extern void netgame_prejoin_players(widget_instance *widget);
extern void mutliplayer_settings_select_list_update_displayed_items(widget_instance *list_widget);
extern void player_profile_3wide_list_update(widget_instance *list_widget);
extern void player_profile_edit_select_menu_update_extended_description(widget_instance *list_widget);
extern void player_profile_1wide_list_update(widget_instance *list_widget);
extern void game_options_menu_update_text_desc(widget_instance *widget);
extern void solo_game_objective_text(widget_instance *widget);
extern void player_profile_color_picker_update(widget_instance *list_widget);
extern void game_options_menu_update_pic_desc(widget_instance *widget);
extern void main_menu_animation_fakery(widget_instance *widget);
extern void mp_level_select_list_update_displayed_items(widget_instance *list_widget);
extern void get_active_player_profile_display_name(widget_instance *widget);
extern void get_editable_player_profile_display_name(widget_instance *widget);
extern void get_editable_playlist_profile_display_name(widget_instance *widget);
extern void get_active_player_profile_color_index(widget_instance *widget);
extern void multiplayer_game_set_text_box_for_map_name(widget_instance *widget);
extern void multiplayer_game_set_text_box_for_game_ruleset(widget_instance *widget);
extern void multiplayer_game_set_text_box_for_teams_noteams(widget_instance *widget);
extern void multiplayer_game_set_text_box_for_score_limit(widget_instance *widget);
extern void multiplayer_game_set_text_box_for_score_limit_type(widget_instance *widget);
extern void multiplayer_game_set_bitmap_for_map(widget_instance *widget);
extern void multiplayer_game_set_bitmap_for_ruleset(widget_instance *widget);
extern void multiplayer_game_set_text_box_for_number_of_players(widget_instance *widget);
extern void multiplayer_edit_profile_set_ruleset_textbox_string_index(widget_instance *widget);
extern void system_link_status_check(widget_instance *widget);
extern void multiplayer_game_directions(widget_instance *widget);
extern void teams_no_teams_mp_game_bitmap_update(widget_instance *widget);
extern void warn_if_difficulty_will_nuke_saved_game(widget_instance *difficulty_screen_widget);
extern void dim_if_no_system_link_cable(widget_instance *widget);
extern void pause_game_set_text_box_for_inverted(widget_instance *widget);
extern void dim_unless_two_controllers(widget_instance *widget);

void (*game_data_input_function_list[0x2B])(widget_instance *widget) = {
    widget_function_null,
    settings_menu_update_extended_description,
    widget_function_null,
    playlist_settings_menu_update_extended_description,
    playlist_gametype_select_menu_update_extended_description,
    multiplayer_type_menu_update_extended_description,
    solo_level_select_list_update_displayed_items,
    difficulty_select_menu_update_extended_description,
    set_textbox_to_build_number,
    server_list_menu_update,
    network_pregame_status_screen_update,
    splitscreen_pregame_status_screen_update,
    netgame_prejoin_players,
    mutliplayer_settings_select_list_update_displayed_items,
    player_profile_3wide_list_update,
    player_profile_edit_select_menu_update_extended_description,
    player_profile_1wide_list_update,
    game_options_menu_update_text_desc,
    solo_game_objective_text,
    player_profile_color_picker_update,
    game_options_menu_update_pic_desc,
    main_menu_animation_fakery,
    mp_level_select_list_update_displayed_items,
    get_active_player_profile_display_name,
    get_editable_player_profile_display_name,
    get_editable_playlist_profile_display_name,
    get_active_player_profile_color_index,
    multiplayer_game_set_text_box_for_map_name,
    multiplayer_game_set_text_box_for_game_ruleset,
    multiplayer_game_set_text_box_for_teams_noteams,
    multiplayer_game_set_text_box_for_score_limit,
    multiplayer_game_set_text_box_for_score_limit_type,
    multiplayer_game_set_bitmap_for_map,
    multiplayer_game_set_bitmap_for_ruleset,
    multiplayer_game_set_text_box_for_number_of_players,
    multiplayer_edit_profile_set_ruleset_textbox_string_index,
    system_link_status_check,
    multiplayer_game_directions,
    teams_no_teams_mp_game_bitmap_update,
    warn_if_difficulty_will_nuke_saved_game,
    dim_if_no_system_link_cable,
    pause_game_set_text_box_for_inverted,
    dim_unless_two_controllers,
};
