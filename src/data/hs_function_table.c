/* hs_function_table @ 0x84178048 (.data, 1884 bytes)
 * DB applied_types: const hs_function_definition *hs_function_table[471];
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x82119008 -> begin_definition
 *   +0x0004 = 0x82119024 -> begin_random_definition
 *   +0x0008 = 0x82119040 -> if_definition
 *   +0x000C = 0x8211905C -> cond_definition
 *   +0x0010 = 0x82119078 -> set_definition
 *   +0x0014 = 0x82119094 -> and_definition
 *   +0x0018 = 0x821190B0 -> or_definition
 *   +0x001C = 0x821190CC -> add_definition
 *   +0x0020 = 0x821190E8 -> subtract_definition
 *   +0x0024 = 0x82119104 -> multiply_definition
 *   +0x0028 = 0x82119120 -> divide_definition
 *   +0x002C = 0x8211913C -> min_definition
 *   +0x0030 = 0x82119158 -> max_definition
 *   +0x0034 = 0x82119174 -> equal_definition
 *   +0x0038 = 0x82119190 -> not_equal_definition
 *   +0x003C = 0x821191AC -> gt_definition
 *   +0x0040 = 0x821191C8 -> lt_definition
 *   +0x0044 = 0x821191E4 -> gte_definition
 *   +0x0048 = 0x82119200 -> lte_definition
 *   +0x004C = 0x8211921C -> sleep_definition
 *   +0x0050 = 0x82119238 -> sleep_until_definition
 *   +0x0054 = 0x82119254 -> wake_definition
 *   +0x0058 = 0x82119270 -> inspect_definition
 *   +0x005C = 0x8211928C -> object_to_unit_definition
 *   +0x0060 = 0x821192A8 -> ai_debug_communication_suppress_definition
 *   +0x0064 = 0x821192C4 -> ai_debug_communication_ignore_definition
 *   +0x0068 = 0x821192E0 -> ai_debug_communication_focus_definition
 *   +0x006C = 0x821192FC -> not_definition
 *   +0x0070 = 0x8211931C -> print_definition
 *   +0x0074 = 0x8211933C -> players_definition
 *   +0x0078 = 0x82119358 -> volume_teleport_players_not_inside_definition
 *   +0x007C = 0x82119378 -> volume_test_object_definition
 *   +0x0080 = 0x82119398 -> volume_test_objects_definition
 *   +0x0084 = 0x821193B8 -> volume_test_objects_all_definition
 *   +0x0088 = 0x821194B4 -> object_teleport_definition
 *   +0x008C = 0x821194D4 -> object_set_facing_definition
 *   +0x0090 = 0x821194F4 -> object_set_shield_definition
 *   +0x0094 = 0x82119514 -> object_set_permutation_definition
 *   +0x0098 = 0x821193D8 -> object_create_definition
 *   +0x009C = 0x821193F8 -> object_destroy_definition
 *   ... 1724 further bytes elided; full hex in .sweep/data_image.tsv
 * data-init const hs_function_definition *[471].
 * HaloScript built-in function/operator definition table, indexed by function opcode.
 * Pointer targets recovered from the binary reloc order (all named .rdata definitions).
 */
#include "../headers/hs_function_definition.h"

extern const hs_function_definition begin_definition;
extern const hs_function_definition begin_random_definition;
extern const hs_function_definition if_definition;
extern const hs_function_definition cond_definition;
extern const hs_function_definition set_definition;
extern const hs_function_definition and_definition;
extern const hs_function_definition or_definition;
extern const hs_function_definition add_definition;
extern const hs_function_definition subtract_definition;
extern const hs_function_definition multiply_definition;
extern const hs_function_definition divide_definition;
extern const hs_function_definition min_definition;
extern const hs_function_definition max_definition;
extern const hs_function_definition equal_definition;
extern const hs_function_definition not_equal_definition;
extern const hs_function_definition gt_definition;
extern const hs_function_definition lt_definition;
extern const hs_function_definition gte_definition;
extern const hs_function_definition lte_definition;
extern const hs_function_definition sleep_definition;
extern const hs_function_definition sleep_until_definition;
extern const hs_function_definition wake_definition;
extern const hs_function_definition inspect_definition;
extern const hs_function_definition object_to_unit_definition;
extern const hs_function_definition ai_debug_communication_suppress_definition;
extern const hs_function_definition ai_debug_communication_ignore_definition;
extern const hs_function_definition ai_debug_communication_focus_definition;
extern const hs_function_definition not_definition;
extern const hs_function_definition print_definition;
extern const hs_function_definition players_definition;
extern const hs_function_definition volume_teleport_players_not_inside_definition;
extern const hs_function_definition volume_test_object_definition;
extern const hs_function_definition volume_test_objects_definition;
extern const hs_function_definition volume_test_objects_all_definition;
extern const hs_function_definition object_teleport_definition;
extern const hs_function_definition object_set_facing_definition;
extern const hs_function_definition object_set_shield_definition;
extern const hs_function_definition object_set_permutation_definition;
extern const hs_function_definition object_create_definition;
extern const hs_function_definition object_destroy_definition;
extern const hs_function_definition object_create_anew_definition;
extern const hs_function_definition object_create_containing_definition;
extern const hs_function_definition object_create_anew_containing_definition;
extern const hs_function_definition object_destroy_containing_definition;
extern const hs_function_definition object_destroy_all_definition;
extern const hs_function_definition list_get_definition;
extern const hs_function_definition list_count_definition;
extern const hs_function_definition effect_new_definition;
extern const hs_function_definition effect_new_on_object_marker_definition;
extern const hs_function_definition damage_new_definition;
extern const hs_function_definition damage_object_definition;
extern const hs_function_definition objects_can_see_object_definition;
extern const hs_function_definition objects_can_see_flag_definition;
extern const hs_function_definition objects_delete_by_definition_definition;
extern const hs_function_definition sound_set_gain_definition;
extern const hs_function_definition sound_get_gain_definition;
extern const hs_function_definition script_recompile_definition;
extern const hs_function_definition script_doc_definition;
extern const hs_function_definition help_definition;
extern const hs_function_definition random_range_definition;
extern const hs_function_definition real_random_range_definition;
extern const hs_function_definition numeric_countdown_timer_set_definition;
extern const hs_function_definition numeric_countdown_timer_get_definition;
extern const hs_function_definition numeric_countdown_timer_stop_definition;
extern const hs_function_definition numeric_countdown_timer_restart_definition;
extern const hs_function_definition breakable_surfaces_enable_definition;
extern const hs_function_definition recording_play_definition;
extern const hs_function_definition recording_play_and_delete_definition;
extern const hs_function_definition recording_play_and_hover_definition;
extern const hs_function_definition recording_kill_definition;
extern const hs_function_definition recording_time_definition;
extern const hs_function_definition object_set_ranged_attack_inhibited_definition;
extern const hs_function_definition object_set_melee_attack_inhibited_definition;
extern const hs_function_definition objects_dump_memory_definition;
extern const hs_function_definition object_set_collideable_definition;
extern const hs_function_definition object_set_scale_definition;
extern const hs_function_definition objects_attach_definition;
extern const hs_function_definition objects_detach_definition;
extern const hs_function_definition garbage_collect_now_definition;
extern const hs_function_definition object_cannot_take_damage_definition;
extern const hs_function_definition object_can_take_damage_definition;
extern const hs_function_definition object_beautify_definition;
extern const hs_function_definition objects_predict_definition;
extern const hs_function_definition object_type_predict_definition;
extern const hs_function_definition object_pvs_activate_definition;
extern const hs_function_definition object_pvs_set_object_definition;
extern const hs_function_definition object_pvs_set_camera_definition;
extern const hs_function_definition object_pvs_clear_definition;
extern const hs_function_definition render_lights_definition;
extern const hs_function_definition scenery_get_animation_time_definition;
extern const hs_function_definition scenery_animation_start_definition;
extern const hs_function_definition scenery_animation_start_at_frame_definition;
extern const hs_function_definition render_effects_definition;
extern const hs_function_definition unit_can_blink_definition;
extern const hs_function_definition unit_open_definition;
extern const hs_function_definition unit_close_definition;
extern const hs_function_definition unit_kill_definition;
extern const hs_function_definition unit_kill_silent_definition;
extern const hs_function_definition unit_get_custom_animation_time_definition;
extern const hs_function_definition unit_stop_custom_animation_definition;
extern const hs_function_definition unit_custom_animation_at_frame_definition;
extern const hs_function_definition custom_animation_definition;
extern const hs_function_definition custom_animation_list_definition;
extern const hs_function_definition unit_is_playing_custom_animation_definition;
extern const hs_function_definition unit_aim_without_turning_definition;
extern const hs_function_definition unit_set_emotion_definition;
extern const hs_function_definition unit_set_enterable_by_player_definition;
extern const hs_function_definition unit_enter_vehicle_definition;
extern const hs_function_definition vehicle_test_seat_list_definition;
extern const hs_function_definition vehicle_test_seat_definition;
extern const hs_function_definition unit_set_emotion_animation_definition;
extern const hs_function_definition unit_exit_vehicle_definition;
extern const hs_function_definition unit_set_maximum_vitality_definition;
extern const hs_function_definition units_set_maximum_vitality_definition;
extern const hs_function_definition unit_set_current_vitality_definition;
extern const hs_function_definition units_set_current_vitality_definition;
extern const hs_function_definition vehicle_load_magic_definition;
extern const hs_function_definition vehicle_unload_definition;
extern const hs_function_definition magic_seat_name_definition;
extern const hs_function_definition unit_set_seat_definition;
extern const hs_function_definition magic_melee_attack_definition;
extern const hs_function_definition vehicle_riders_definition;
extern const hs_function_definition vehicle_driver_definition;
extern const hs_function_definition vehicle_gunner_definition;
extern const hs_function_definition unit_get_health_definition;
extern const hs_function_definition unit_get_shield_definition;
extern const hs_function_definition unit_get_total_grenade_count_definition;
extern const hs_function_definition unit_has_weapon_definition;
extern const hs_function_definition unit_has_weapon_readied_definition;
extern const hs_function_definition unit_doesnt_drop_items_definition;
extern const hs_function_definition unit_impervious_definition;
extern const hs_function_definition unit_suspended_definition;
extern const hs_function_definition unit_solo_player_integrated_night_vision_is_active_definition;
extern const hs_function_definition units_set_desired_flashlight_state_definition;
extern const hs_function_definition unit_set_desired_flashlight_state_definition;
extern const hs_function_definition unit_get_current_flashlight_state_definition;
extern const hs_function_definition device_set_never_appears_locked_definition;
extern const hs_function_definition device_get_power_definition;
extern const hs_function_definition device_set_power_definition;
extern const hs_function_definition device_set_position_definition;
extern const hs_function_definition device_get_position_definition;
extern const hs_function_definition device_set_position_immediate_definition;
extern const hs_function_definition device_group_get_definition;
extern const hs_function_definition device_group_set_definition;
extern const hs_function_definition device_group_set_immediate_definition;
extern const hs_function_definition device_one_sided_set_definition;
extern const hs_function_definition device_operates_automatically_set_definition;
extern const hs_function_definition device_group_change_only_once_more_set_definition;
extern const hs_function_definition breakable_surfaces_reset_definition;
extern const hs_function_definition cheat_all_powerups_definition;
extern const hs_function_definition cheat_all_weapons_definition;
extern const hs_function_definition cheat_spawn_warthog_definition;
extern const hs_function_definition cheat_all_vehicles_definition;
extern const hs_function_definition cheat_teleport_to_camera_definition;
extern const hs_function_definition cheat_active_camouflage_definition;
extern const hs_function_definition cheat_active_camouflage_local_player_definition;
extern const hs_function_definition cheats_load_definition;
extern const hs_function_definition ai_free_definition;
extern const hs_function_definition ai_free_units_definition;
extern const hs_function_definition ai_attach_definition;
extern const hs_function_definition ai_attach_free_definition;
extern const hs_function_definition ai_detach_definition;
extern const hs_function_definition ai_place_definition;
extern const hs_function_definition ai_kill_definition;
extern const hs_function_definition ai_kill_silent_definition;
extern const hs_function_definition ai_erase_definition;
extern const hs_function_definition ai_erase_all_definition;
extern const hs_function_definition ai_select_definition;
extern const hs_function_definition ai_deselect_definition;
extern const hs_function_definition ai_spawn_actor_definition;
extern const hs_function_definition ai_set_respawn_definition;
extern const hs_function_definition ai_set_deaf_definition;
extern const hs_function_definition ai_set_blind_definition;
extern const hs_function_definition ai_magically_see_encounter_definition;
extern const hs_function_definition ai_magically_see_players_definition;
extern const hs_function_definition ai_magically_see_unit_definition;
extern const hs_function_definition ai_timer_start_definition;
extern const hs_function_definition ai_timer_expire_definition;
extern const hs_function_definition ai_attack_definition;
extern const hs_function_definition ai_defend_definition;
extern const hs_function_definition ai_retreat_definition;
extern const hs_function_definition ai_maneuver_definition;
extern const hs_function_definition ai_maneuver_enable_definition;
extern const hs_function_definition ai_migrate_definition;
extern const hs_function_definition ai_migrate_and_speak_definition;
extern const hs_function_definition ai_migrate_by_unit_definition;
extern const hs_function_definition ai_allegiance_definition;
extern const hs_function_definition ai_allegiance_remove_definition;
extern const hs_function_definition ai_living_count_definition;
extern const hs_function_definition ai_living_fraction_definition;
extern const hs_function_definition ai_strength_definition;
extern const hs_function_definition ai_swarm_count_definition;
extern const hs_function_definition ai_nonswarm_count_definition;
extern const hs_function_definition ai_actors_definition;
extern const hs_function_definition ai_go_to_vehicle_definition;
extern const hs_function_definition ai_go_to_vehicle_override_definition;
extern const hs_function_definition ai_going_to_vehicle_definition;
extern const hs_function_definition ai_exit_vehicle_definition;
extern const hs_function_definition ai_braindead_definition;
extern const hs_function_definition ai_braindead_by_unit_definition;
extern const hs_function_definition ai_disregard_definition;
extern const hs_function_definition ai_prefer_target_definition;
extern const hs_function_definition ai_teleport_to_starting_location_definition;
extern const hs_function_definition ai_teleport_to_starting_location_if_unsupported_definition;
extern const hs_function_definition ai_renew_definition;
extern const hs_function_definition ai_try_to_fight_nothing_definition;
extern const hs_function_definition ai_try_to_fight_definition;
extern const hs_function_definition ai_try_to_fight_player_definition;
extern const hs_function_definition ai_command_list_definition;
extern const hs_function_definition ai_command_list_by_unit_definition;
extern const hs_function_definition ai_command_list_advance_definition;
extern const hs_function_definition ai_command_list_advance_by_unit_definition;
extern const hs_function_definition ai_command_list_status_definition;
extern const hs_function_definition ai_is_attacking_definition;
extern const hs_function_definition ai_force_active_definition;
extern const hs_function_definition ai_force_active_by_unit_definition;
extern const hs_function_definition ai_set_return_state_definition;
extern const hs_function_definition ai_set_current_state_definition;
extern const hs_function_definition ai_playfight_definition;
extern const hs_function_definition ai_status_definition;
extern const hs_function_definition ai_reconnect_definition;
extern const hs_function_definition ai_vehicle_encounter_definition;
extern const hs_function_definition ai_vehicle_enterable_distance_definition;
extern const hs_function_definition ai_vehicle_enterable_team_definition;
extern const hs_function_definition ai_vehicle_enterable_actor_type_definition;
extern const hs_function_definition ai_vehicle_enterable_actors_definition;
extern const hs_function_definition ai_vehicle_enterable_disable_definition;
extern const hs_function_definition ai_look_at_object_definition;
extern const hs_function_definition ai_stop_looking_definition;
extern const hs_function_definition ai_automatic_migration_target_definition;
extern const hs_function_definition ai_follow_target_disable_definition;
extern const hs_function_definition ai_follow_target_players_definition;
extern const hs_function_definition ai_follow_target_unit_definition;
extern const hs_function_definition ai_follow_target_ai_definition;
extern const hs_function_definition ai_follow_distance_definition;
extern const hs_function_definition ai_conversation_definition;
extern const hs_function_definition ai_conversation_stop_definition;
extern const hs_function_definition ai_conversation_advance_definition;
extern const hs_function_definition ai_conversation_line_definition;
extern const hs_function_definition ai_conversation_status_definition;
extern const hs_function_definition ai_link_activation_definition;
extern const hs_function_definition ai_berserk_definition;
extern const hs_function_definition ai_set_team_definition;
extern const hs_function_definition ai_allow_charge_definition;
extern const hs_function_definition ai_allow_dormant_definition;
extern const hs_function_definition ai_allegiance_broken_definition;
extern const hs_function_definition camera_control_definition;
extern const hs_function_definition camera_set_definition;
extern const hs_function_definition camera_set_relative_definition;
extern const hs_function_definition camera_set_animation_definition;
extern const hs_function_definition camera_set_first_person_definition;
extern const hs_function_definition camera_set_dead_definition;
extern const hs_function_definition camera_time_definition;
extern const hs_function_definition debug_camera_load_definition;
extern const hs_function_definition debug_camera_save_definition;
extern const hs_function_definition game_speed_definition;
extern const hs_function_definition game_time_definition;
extern const hs_function_definition game_variant_definition;
extern const hs_function_definition game_difficulty_get_definition;
extern const hs_function_definition game_difficulty_get_real_definition;
extern const hs_function_definition profile_service_clear_timers_definition;
extern const hs_function_definition profile_service_dump_timers_definition;
extern const hs_function_definition map_reset_definition;
extern const hs_function_definition map_name_definition;
extern const hs_function_definition multiplayer_map_name_definition;
extern const hs_function_definition game_difficulty_set_definition;
extern const hs_function_definition crash_definition;
extern const hs_function_definition switch_bsp_definition;
extern const hs_function_definition structure_bsp_index_definition;
extern const hs_function_definition version_definition;
extern const hs_function_definition playback_definition;
extern const hs_function_definition quit_definition;
extern const hs_function_definition texture_cache_flush_definition;
extern const hs_function_definition sound_cache_flush_definition;
extern const hs_function_definition debug_memory_definition;
extern const hs_function_definition debug_memory_by_file_definition;
extern const hs_function_definition debug_memory_for_file_definition;
extern const hs_function_definition debug_tags_definition;
extern const hs_function_definition profile_reset_definition;
extern const hs_function_definition profile_dump_definition;
extern const hs_function_definition profile_activate_definition;
extern const hs_function_definition profile_deactivate_definition;
extern const hs_function_definition profile_graph_toggle_definition;
extern const hs_function_definition ai_definition;
extern const hs_function_definition ai_dialogue_triggers_definition;
extern const hs_function_definition ai_grenades_definition;
extern const hs_function_definition ai_lines_definition;
extern const hs_function_definition ai_debug_sound_point_set_definition;
extern const hs_function_definition ai_debug_vocalize_definition;
extern const hs_function_definition ai_debug_teleport_to_definition;
extern const hs_function_definition ai_debug_speak_definition;
extern const hs_function_definition ai_debug_speak_list_definition;
extern const hs_function_definition fade_in_definition;
extern const hs_function_definition fade_out_definition;
extern const hs_function_definition cinematic_start_definition;
extern const hs_function_definition cinematic_stop_definition;
extern const hs_function_definition cinematic_abort_definition;
extern const hs_function_definition cinematic_skip_start_internal_definition;
extern const hs_function_definition cinematic_skip_stop_internal_definition;
extern const hs_function_definition cinematic_show_letterbox_definition;
extern const hs_function_definition cinematic_set_title_definition;
extern const hs_function_definition cinematic_set_title_delayed_definition;
extern const hs_function_definition cinematic_suppress_bsp_object_creation_definition;
extern const hs_function_definition attract_mode_start_definition;
extern const hs_function_definition game_won_definition;
extern const hs_function_definition game_lost_definition;
extern const hs_function_definition game_safe_to_save_definition;
extern const hs_function_definition game_all_quiet_definition;
extern const hs_function_definition game_safe_to_speak_definition;
extern const hs_function_definition game_is_cooperative_definition;
extern const hs_function_definition game_save_definition;
extern const hs_function_definition game_save_cancel_definition;
extern const hs_function_definition game_save_no_timeout_definition;
extern const hs_function_definition game_save_totally_unsafe_definition;
extern const hs_function_definition game_saving_definition;
extern const hs_function_definition game_revert_definition;
extern const hs_function_definition game_reverted_definition;
extern const hs_function_definition core_save_definition;
extern const hs_function_definition core_save_name_definition;
extern const hs_function_definition core_load_definition;
extern const hs_function_definition core_load_at_startup_definition;
extern const hs_function_definition core_load_name_definition;
extern const hs_function_definition core_load_name_at_startup_definition;
extern const hs_function_definition game_skip_ticks_definition;
extern const hs_function_definition sound_impulse_predict_definition;
extern const hs_function_definition sound_impulse_start_definition;
extern const hs_function_definition sound_impulse_time_definition;
extern const hs_function_definition sound_impulse_stop_definition;
extern const hs_function_definition sound_looping_predict_definition;
extern const hs_function_definition sound_looping_start_definition;
extern const hs_function_definition sound_looping_stop_definition;
extern const hs_function_definition sound_looping_set_scale_definition;
extern const hs_function_definition sound_looping_set_alternate_definition;
extern const hs_function_definition debug_sounds_enable_definition;
extern const hs_function_definition debug_sounds_distances_definition;
extern const hs_function_definition debug_sounds_wet_definition;
extern const hs_function_definition sound_enable_definition;
extern const hs_function_definition sound_set_master_gain_definition;
extern const hs_function_definition sound_get_master_gain_definition;
extern const hs_function_definition sound_set_music_gain_definition;
extern const hs_function_definition sound_get_music_gain_definition;
extern const hs_function_definition sound_set_effects_gain_definition;
extern const hs_function_definition sound_get_effects_gain_definition;
extern const hs_function_definition sound_class_set_gain_definition;
extern const hs_function_definition vehicle_hover_definition;
extern const hs_function_definition players_unzoom_all_definition;
extern const hs_function_definition player_enable_input_definition;
extern const hs_function_definition player_camera_control_definition;
extern const hs_function_definition player_action_test_reset_definition;
extern const hs_function_definition player_action_test_jump_definition;
extern const hs_function_definition player_action_test_primary_trigger_definition;
extern const hs_function_definition player_action_test_grenade_trigger_definition;
extern const hs_function_definition player_action_test_zoom_definition;
extern const hs_function_definition player_action_test_action_definition;
extern const hs_function_definition player_action_test_accept_definition;
extern const hs_function_definition player_action_test_back_definition;
extern const hs_function_definition player_action_test_look_relative_up_definition;
extern const hs_function_definition player_action_test_look_relative_down_definition;
extern const hs_function_definition player_action_test_look_relative_left_definition;
extern const hs_function_definition player_action_test_look_relative_right_definition;
extern const hs_function_definition player_action_test_look_relative_all_directions_definition;
extern const hs_function_definition player_action_test_move_relative_all_directions_definition;
extern const hs_function_definition player_add_equipment_definition;
extern const hs_function_definition show_hud_definition;
extern const hs_function_definition show_hud_help_text_definition;
extern const hs_function_definition enable_hud_help_flash_definition;
extern const hs_function_definition hud_help_flash_restart_definition;
extern const hs_function_definition activate_nav_point_flag_definition;
extern const hs_function_definition activate_nav_point_object_definition;
extern const hs_function_definition activate_team_nav_point_flag_definition;
extern const hs_function_definition activate_team_nav_point_object_definition;
extern const hs_function_definition deactivate_nav_point_flag_definition;
extern const hs_function_definition deactivate_nav_point_object_definition;
extern const hs_function_definition deactivate_team_nav_point_flag_definition;
extern const hs_function_definition deactivate_team_nav_point_object_definition;
extern const hs_function_definition cls_definition;
extern const hs_function_definition connect_definition;
extern const hs_function_definition disconnect_definition;
extern const hs_function_definition hammer_begin_definition;
extern const hs_function_definition hammer_stop_definition;
extern const hs_function_definition network_server_dump_definition;
extern const hs_function_definition network_client_dump_definition;
extern const hs_function_definition net_graph_clear_definition;
extern const hs_function_definition net_graph_show_definition;
extern const hs_function_definition play_update_history_definition;
extern const hs_function_definition show_player_update_stats_definition;
extern const hs_function_definition message_metrics_clear_definition;
extern const hs_function_definition message_metrics_dump_definition;
extern const hs_function_definition error_overflow_suppression_definition;
extern const hs_function_definition structure_lens_flares_place_definition;
extern const hs_function_definition player_effect_set_max_translation_definition;
extern const hs_function_definition player_effect_set_max_rotation_definition;
extern const hs_function_definition player_effect_set_max_vibrate_definition;
extern const hs_function_definition player_effect_start_definition;
extern const hs_function_definition player_effect_stop_definition;
extern const hs_function_definition hud_show_health_definition;
extern const hs_function_definition hud_blink_health_definition;
extern const hs_function_definition hud_show_shield_definition;
extern const hs_function_definition hud_blink_shield_definition;
extern const hs_function_definition hud_show_motion_sensor_definition;
extern const hs_function_definition hud_blink_motion_sensor_definition;
extern const hs_function_definition hud_show_crosshair_definition;
extern const hs_function_definition hud_clear_messages_definition;
extern const hs_function_definition hud_set_help_text_definition;
extern const hs_function_definition hud_set_objective_text_definition;
extern const hs_function_definition hud_set_timer_time_definition;
extern const hs_function_definition hud_set_timer_warning_time_definition;
extern const hs_function_definition hud_set_timer_position_definition;
extern const hs_function_definition show_hud_timer_definition;
extern const hs_function_definition pause_hud_timer_definition;
extern const hs_function_definition hud_get_timer_ticks_definition;
extern const hs_function_definition time_code_show_definition;
extern const hs_function_definition time_code_start_definition;
extern const hs_function_definition time_code_reset_definition;
extern const hs_function_definition set_gamma_definition;
extern const hs_function_definition rasterizer_fixed_function_ambient_definition;
extern const hs_function_definition rasterizer_decals_flush_definition;
extern const hs_function_definition rasterizer_fps_accumulate_definition;
extern const hs_function_definition rasterizer_model_ambient_reflection_tint_definition;
extern const hs_function_definition rasterizer_lights_reset_for_new_map_definition;
extern const hs_function_definition script_screen_effect_set_value_definition;
extern const hs_function_definition cinematic_screen_effect_start_definition;
extern const hs_function_definition cinematic_screen_effect_set_convolution_definition;
extern const hs_function_definition cinematic_screen_effect_set_filter_definition;
extern const hs_function_definition cinematic_screen_effect_set_filter_desaturation_tint_definition;
extern const hs_function_definition cinematic_screen_effect_set_video_definition;
extern const hs_function_definition cinematic_screen_effect_stop_definition;
extern const hs_function_definition cinematic_set_near_clip_distance_definition;
extern const hs_function_definition delete_save_game_files_definition;
extern const hs_function_definition fast_setup_network_server_definition;
extern const hs_function_definition profile_unlock_solo_levels_definition;
extern const hs_function_definition player0_look_invert_pitch_definition;
extern const hs_function_definition player0_look_pitch_is_inverted_definition;
extern const hs_function_definition player0_joystick_set_is_normal_definition;
extern const hs_function_definition ui_widget_show_path_definition;
extern const hs_function_definition display_scenario_help_definition;
extern const hs_function_definition xbox_set_machine_name_definition;
extern const hs_function_definition input_find_default_definition;
extern const hs_function_definition config_one_control_definition;
extern const hs_function_definition get_yaw_rate_definition;
extern const hs_function_definition get_pitch_rate_definition;
extern const hs_function_definition set_yaw_rate_definition;
extern const hs_function_definition set_pitch_rate_definition;
extern const hs_function_definition bind_definition;
extern const hs_function_definition unbind_definition;
extern const hs_function_definition print_binds_definition;
extern const hs_function_definition sv_end_game_definition;
extern const hs_function_definition change_team_definition;
extern const hs_function_definition sv_mapcycle_definition;
extern const hs_function_definition sv_mapcycle_begin_definition;
extern const hs_function_definition sv_mapcycle_save_definition;
extern const hs_function_definition sv_mapcycle_add_definition;
extern const hs_function_definition sv_mapcycle_del_definition;
extern const hs_function_definition sv_map_next_definition;
extern const hs_function_definition sv_map_restart_definition;
extern const hs_function_definition sv_map_definition;
extern const hs_function_definition sv_players_definition;
extern const hs_function_definition sv_kick_definition;
extern const hs_function_definition sv_ban_definition;
extern const hs_function_definition sv_banlist_definition;
extern const hs_function_definition sv_unban_definition;
extern const hs_function_definition sv_parameters_reload_definition;
extern const hs_function_definition sv_parameters_dump_definition;
extern const hs_function_definition sv_status_definition;
extern const hs_function_definition sv_name_definition;
extern const hs_function_definition sv_password_definition;
extern const hs_function_definition profile_load_definition;
extern const hs_function_definition track_remote_player_position_updates_definition;
extern const hs_function_definition thread_sleep_definition;
extern const hs_function_definition checkpoint_save_definition;
extern const hs_function_definition checkpoint_load_definition;

const hs_function_definition *hs_function_table[471] =
{
    &begin_definition,
    &begin_random_definition,
    &if_definition,
    &cond_definition,
    &set_definition,
    &and_definition,
    &or_definition,
    &add_definition,
    &subtract_definition,
    &multiply_definition,
    &divide_definition,
    &min_definition,
    &max_definition,
    &equal_definition,
    &not_equal_definition,
    &gt_definition,
    &lt_definition,
    &gte_definition,
    &lte_definition,
    &sleep_definition,
    &sleep_until_definition,
    &wake_definition,
    &inspect_definition,
    &object_to_unit_definition,
    &ai_debug_communication_suppress_definition,
    &ai_debug_communication_ignore_definition,
    &ai_debug_communication_focus_definition,
    &not_definition,
    &print_definition,
    &players_definition,
    &volume_teleport_players_not_inside_definition,
    &volume_test_object_definition,
    &volume_test_objects_definition,
    &volume_test_objects_all_definition,
    &object_teleport_definition,
    &object_set_facing_definition,
    &object_set_shield_definition,
    &object_set_permutation_definition,
    &object_create_definition,
    &object_destroy_definition,
    &object_create_anew_definition,
    &object_create_containing_definition,
    &object_create_anew_containing_definition,
    &object_destroy_containing_definition,
    &object_destroy_all_definition,
    &list_get_definition,
    &list_count_definition,
    &effect_new_definition,
    &effect_new_on_object_marker_definition,
    &damage_new_definition,
    &damage_object_definition,
    &objects_can_see_object_definition,
    &objects_can_see_flag_definition,
    &objects_delete_by_definition_definition,
    &sound_set_gain_definition,
    &sound_get_gain_definition,
    &script_recompile_definition,
    &script_doc_definition,
    &help_definition,
    &random_range_definition,
    &real_random_range_definition,
    &numeric_countdown_timer_set_definition,
    &numeric_countdown_timer_get_definition,
    &numeric_countdown_timer_stop_definition,
    &numeric_countdown_timer_restart_definition,
    &breakable_surfaces_enable_definition,
    &recording_play_definition,
    &recording_play_and_delete_definition,
    &recording_play_and_hover_definition,
    &recording_kill_definition,
    &recording_time_definition,
    &object_set_ranged_attack_inhibited_definition,
    &object_set_melee_attack_inhibited_definition,
    &objects_dump_memory_definition,
    &object_set_collideable_definition,
    &object_set_scale_definition,
    &objects_attach_definition,
    &objects_detach_definition,
    &garbage_collect_now_definition,
    &object_cannot_take_damage_definition,
    &object_can_take_damage_definition,
    &object_beautify_definition,
    &objects_predict_definition,
    &object_type_predict_definition,
    &object_pvs_activate_definition,
    &object_pvs_set_object_definition,
    &object_pvs_set_camera_definition,
    &object_pvs_clear_definition,
    &render_lights_definition,
    &scenery_get_animation_time_definition,
    &scenery_animation_start_definition,
    &scenery_animation_start_at_frame_definition,
    &render_effects_definition,
    &unit_can_blink_definition,
    &unit_open_definition,
    &unit_close_definition,
    &unit_kill_definition,
    &unit_kill_silent_definition,
    &unit_get_custom_animation_time_definition,
    &unit_stop_custom_animation_definition,
    &unit_custom_animation_at_frame_definition,
    &custom_animation_definition,
    &custom_animation_list_definition,
    &unit_is_playing_custom_animation_definition,
    &unit_aim_without_turning_definition,
    &unit_set_emotion_definition,
    &unit_set_enterable_by_player_definition,
    &unit_enter_vehicle_definition,
    &vehicle_test_seat_list_definition,
    &vehicle_test_seat_definition,
    &unit_set_emotion_animation_definition,
    &unit_exit_vehicle_definition,
    &unit_set_maximum_vitality_definition,
    &units_set_maximum_vitality_definition,
    &unit_set_current_vitality_definition,
    &units_set_current_vitality_definition,
    &vehicle_load_magic_definition,
    &vehicle_unload_definition,
    &magic_seat_name_definition,
    &unit_set_seat_definition,
    &magic_melee_attack_definition,
    &vehicle_riders_definition,
    &vehicle_driver_definition,
    &vehicle_gunner_definition,
    &unit_get_health_definition,
    &unit_get_shield_definition,
    &unit_get_total_grenade_count_definition,
    &unit_has_weapon_definition,
    &unit_has_weapon_readied_definition,
    &unit_doesnt_drop_items_definition,
    &unit_impervious_definition,
    &unit_suspended_definition,
    &unit_solo_player_integrated_night_vision_is_active_definition,
    &units_set_desired_flashlight_state_definition,
    &unit_set_desired_flashlight_state_definition,
    &unit_get_current_flashlight_state_definition,
    &device_set_never_appears_locked_definition,
    &device_get_power_definition,
    &device_set_power_definition,
    &device_set_position_definition,
    &device_get_position_definition,
    &device_set_position_immediate_definition,
    &device_group_get_definition,
    &device_group_set_definition,
    &device_group_set_immediate_definition,
    &device_one_sided_set_definition,
    &device_operates_automatically_set_definition,
    &device_group_change_only_once_more_set_definition,
    &breakable_surfaces_reset_definition,
    &cheat_all_powerups_definition,
    &cheat_all_weapons_definition,
    &cheat_spawn_warthog_definition,
    &cheat_all_vehicles_definition,
    &cheat_teleport_to_camera_definition,
    &cheat_active_camouflage_definition,
    &cheat_active_camouflage_local_player_definition,
    &cheats_load_definition,
    &ai_free_definition,
    &ai_free_units_definition,
    &ai_attach_definition,
    &ai_attach_free_definition,
    &ai_detach_definition,
    &ai_place_definition,
    &ai_kill_definition,
    &ai_kill_silent_definition,
    &ai_erase_definition,
    &ai_erase_all_definition,
    &ai_select_definition,
    &ai_deselect_definition,
    &ai_spawn_actor_definition,
    &ai_set_respawn_definition,
    &ai_set_deaf_definition,
    &ai_set_blind_definition,
    &ai_magically_see_encounter_definition,
    &ai_magically_see_players_definition,
    &ai_magically_see_unit_definition,
    &ai_timer_start_definition,
    &ai_timer_expire_definition,
    &ai_attack_definition,
    &ai_defend_definition,
    &ai_retreat_definition,
    &ai_maneuver_definition,
    &ai_maneuver_enable_definition,
    &ai_migrate_definition,
    &ai_migrate_and_speak_definition,
    &ai_migrate_by_unit_definition,
    &ai_allegiance_definition,
    &ai_allegiance_remove_definition,
    &ai_living_count_definition,
    &ai_living_fraction_definition,
    &ai_strength_definition,
    &ai_swarm_count_definition,
    &ai_nonswarm_count_definition,
    &ai_actors_definition,
    &ai_go_to_vehicle_definition,
    &ai_go_to_vehicle_override_definition,
    &ai_going_to_vehicle_definition,
    &ai_exit_vehicle_definition,
    &ai_braindead_definition,
    &ai_braindead_by_unit_definition,
    &ai_disregard_definition,
    &ai_prefer_target_definition,
    &ai_teleport_to_starting_location_definition,
    &ai_teleport_to_starting_location_if_unsupported_definition,
    &ai_renew_definition,
    &ai_try_to_fight_nothing_definition,
    &ai_try_to_fight_definition,
    &ai_try_to_fight_player_definition,
    &ai_command_list_definition,
    &ai_command_list_by_unit_definition,
    &ai_command_list_advance_definition,
    &ai_command_list_advance_by_unit_definition,
    &ai_command_list_status_definition,
    &ai_is_attacking_definition,
    &ai_force_active_definition,
    &ai_force_active_by_unit_definition,
    &ai_set_return_state_definition,
    &ai_set_current_state_definition,
    &ai_playfight_definition,
    &ai_status_definition,
    &ai_reconnect_definition,
    &ai_vehicle_encounter_definition,
    &ai_vehicle_enterable_distance_definition,
    &ai_vehicle_enterable_team_definition,
    &ai_vehicle_enterable_actor_type_definition,
    &ai_vehicle_enterable_actors_definition,
    &ai_vehicle_enterable_disable_definition,
    &ai_look_at_object_definition,
    &ai_stop_looking_definition,
    &ai_automatic_migration_target_definition,
    &ai_follow_target_disable_definition,
    &ai_follow_target_players_definition,
    &ai_follow_target_unit_definition,
    &ai_follow_target_ai_definition,
    &ai_follow_distance_definition,
    &ai_conversation_definition,
    &ai_conversation_stop_definition,
    &ai_conversation_advance_definition,
    &ai_conversation_line_definition,
    &ai_conversation_status_definition,
    &ai_link_activation_definition,
    &ai_berserk_definition,
    &ai_set_team_definition,
    &ai_allow_charge_definition,
    &ai_allow_dormant_definition,
    &ai_allegiance_broken_definition,
    &camera_control_definition,
    &camera_set_definition,
    &camera_set_relative_definition,
    &camera_set_animation_definition,
    &camera_set_first_person_definition,
    &camera_set_dead_definition,
    &camera_time_definition,
    &debug_camera_load_definition,
    &debug_camera_save_definition,
    &game_speed_definition,
    &game_time_definition,
    &game_variant_definition,
    &game_difficulty_get_definition,
    &game_difficulty_get_real_definition,
    &profile_service_clear_timers_definition,
    &profile_service_dump_timers_definition,
    &map_reset_definition,
    &map_name_definition,
    &multiplayer_map_name_definition,
    &game_difficulty_set_definition,
    &crash_definition,
    &switch_bsp_definition,
    &structure_bsp_index_definition,
    &version_definition,
    &playback_definition,
    &quit_definition,
    &texture_cache_flush_definition,
    &sound_cache_flush_definition,
    &debug_memory_definition,
    &debug_memory_by_file_definition,
    &debug_memory_for_file_definition,
    &debug_tags_definition,
    &profile_reset_definition,
    &profile_dump_definition,
    &profile_activate_definition,
    &profile_deactivate_definition,
    &profile_graph_toggle_definition,
    &ai_definition,
    &ai_dialogue_triggers_definition,
    &ai_grenades_definition,
    &ai_lines_definition,
    &ai_debug_sound_point_set_definition,
    &ai_debug_vocalize_definition,
    &ai_debug_teleport_to_definition,
    &ai_debug_speak_definition,
    &ai_debug_speak_list_definition,
    &fade_in_definition,
    &fade_out_definition,
    &cinematic_start_definition,
    &cinematic_stop_definition,
    &cinematic_abort_definition,
    &cinematic_skip_start_internal_definition,
    &cinematic_skip_stop_internal_definition,
    &cinematic_show_letterbox_definition,
    &cinematic_set_title_definition,
    &cinematic_set_title_delayed_definition,
    &cinematic_suppress_bsp_object_creation_definition,
    &attract_mode_start_definition,
    &game_won_definition,
    &game_lost_definition,
    &game_safe_to_save_definition,
    &game_all_quiet_definition,
    &game_safe_to_speak_definition,
    &game_is_cooperative_definition,
    &game_save_definition,
    &game_save_cancel_definition,
    &game_save_no_timeout_definition,
    &game_save_totally_unsafe_definition,
    &game_saving_definition,
    &game_revert_definition,
    &game_reverted_definition,
    &core_save_definition,
    &core_save_name_definition,
    &core_load_definition,
    &core_load_at_startup_definition,
    &core_load_name_definition,
    &core_load_name_at_startup_definition,
    &game_skip_ticks_definition,
    &sound_impulse_predict_definition,
    &sound_impulse_start_definition,
    &sound_impulse_time_definition,
    &sound_impulse_stop_definition,
    &sound_looping_predict_definition,
    &sound_looping_start_definition,
    &sound_looping_stop_definition,
    &sound_looping_set_scale_definition,
    &sound_looping_set_alternate_definition,
    &debug_sounds_enable_definition,
    &debug_sounds_distances_definition,
    &debug_sounds_wet_definition,
    &sound_enable_definition,
    &sound_set_master_gain_definition,
    &sound_get_master_gain_definition,
    &sound_set_music_gain_definition,
    &sound_get_music_gain_definition,
    &sound_set_effects_gain_definition,
    &sound_get_effects_gain_definition,
    &sound_class_set_gain_definition,
    &vehicle_hover_definition,
    &players_unzoom_all_definition,
    &player_enable_input_definition,
    &player_camera_control_definition,
    &player_action_test_reset_definition,
    &player_action_test_jump_definition,
    &player_action_test_primary_trigger_definition,
    &player_action_test_grenade_trigger_definition,
    &player_action_test_zoom_definition,
    &player_action_test_action_definition,
    &player_action_test_accept_definition,
    &player_action_test_back_definition,
    &player_action_test_look_relative_up_definition,
    &player_action_test_look_relative_down_definition,
    &player_action_test_look_relative_left_definition,
    &player_action_test_look_relative_right_definition,
    &player_action_test_look_relative_all_directions_definition,
    &player_action_test_move_relative_all_directions_definition,
    &player_add_equipment_definition,
    &show_hud_definition,
    &show_hud_help_text_definition,
    &enable_hud_help_flash_definition,
    &hud_help_flash_restart_definition,
    &activate_nav_point_flag_definition,
    &activate_nav_point_object_definition,
    &activate_team_nav_point_flag_definition,
    &activate_team_nav_point_object_definition,
    &deactivate_nav_point_flag_definition,
    &deactivate_nav_point_object_definition,
    &deactivate_team_nav_point_flag_definition,
    &deactivate_team_nav_point_object_definition,
    &cls_definition,
    &connect_definition,
    &disconnect_definition,
    &hammer_begin_definition,
    &hammer_stop_definition,
    &network_server_dump_definition,
    &network_client_dump_definition,
    &net_graph_clear_definition,
    &net_graph_show_definition,
    &play_update_history_definition,
    &show_player_update_stats_definition,
    &message_metrics_clear_definition,
    &message_metrics_dump_definition,
    &error_overflow_suppression_definition,
    &structure_lens_flares_place_definition,
    &player_effect_set_max_translation_definition,
    &player_effect_set_max_rotation_definition,
    &player_effect_set_max_vibrate_definition,
    &player_effect_start_definition,
    &player_effect_stop_definition,
    &hud_show_health_definition,
    &hud_blink_health_definition,
    &hud_show_shield_definition,
    &hud_blink_shield_definition,
    &hud_show_motion_sensor_definition,
    &hud_blink_motion_sensor_definition,
    &hud_show_crosshair_definition,
    &hud_clear_messages_definition,
    &hud_set_help_text_definition,
    &hud_set_objective_text_definition,
    &hud_set_timer_time_definition,
    &hud_set_timer_warning_time_definition,
    &hud_set_timer_position_definition,
    &show_hud_timer_definition,
    &pause_hud_timer_definition,
    &hud_get_timer_ticks_definition,
    &time_code_show_definition,
    &time_code_start_definition,
    &time_code_reset_definition,
    &set_gamma_definition,
    &rasterizer_fixed_function_ambient_definition,
    &rasterizer_decals_flush_definition,
    &rasterizer_fps_accumulate_definition,
    &rasterizer_model_ambient_reflection_tint_definition,
    &rasterizer_lights_reset_for_new_map_definition,
    &script_screen_effect_set_value_definition,
    &cinematic_screen_effect_start_definition,
    &cinematic_screen_effect_set_convolution_definition,
    &cinematic_screen_effect_set_filter_definition,
    &cinematic_screen_effect_set_filter_desaturation_tint_definition,
    &cinematic_screen_effect_set_video_definition,
    &cinematic_screen_effect_stop_definition,
    &cinematic_set_near_clip_distance_definition,
    &delete_save_game_files_definition,
    &fast_setup_network_server_definition,
    &profile_unlock_solo_levels_definition,
    &player0_look_invert_pitch_definition,
    &player0_look_pitch_is_inverted_definition,
    &player0_joystick_set_is_normal_definition,
    &ui_widget_show_path_definition,
    &display_scenario_help_definition,
    &xbox_set_machine_name_definition,
    &input_find_default_definition,
    &config_one_control_definition,
    &get_yaw_rate_definition,
    &get_pitch_rate_definition,
    &set_yaw_rate_definition,
    &set_pitch_rate_definition,
    &bind_definition,
    &unbind_definition,
    &print_binds_definition,
    &sv_end_game_definition,
    &change_team_definition,
    &sv_mapcycle_definition,
    &sv_mapcycle_begin_definition,
    &sv_mapcycle_save_definition,
    &sv_mapcycle_add_definition,
    &sv_mapcycle_del_definition,
    &sv_map_next_definition,
    &sv_map_restart_definition,
    &sv_map_definition,
    &sv_players_definition,
    &sv_kick_definition,
    &sv_ban_definition,
    &sv_banlist_definition,
    &sv_unban_definition,
    &sv_parameters_reload_definition,
    &sv_parameters_dump_definition,
    &sv_status_definition,
    &sv_name_definition,
    &sv_password_definition,
    &profile_load_definition,
    &track_remote_player_position_updates_definition,
    &thread_sleep_definition,
    &checkpoint_save_definition,
    &checkpoint_load_definition,
};
