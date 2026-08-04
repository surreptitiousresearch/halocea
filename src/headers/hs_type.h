#ifndef HS_TYPE_H
#define HS_TYPE_H

/* hs_type — HaloScript syntax-node / value type (hs_syntax_node.type).
 * Source: DB enum _D0F641CA8E0315935795906396F6347C (types_enum_values; also headers_ref).
 * Range markers (FIRST_, NOT_LAST_, LAST_, NUMBER_OF_) kept verbatim, in DB value_index
 * order, since compile code range-tests against them. */
enum hs_type {
	hs_unparsed = 0,
	hs_special_form = 1,
	hs_function_name = 2,
	hs_passthrough = 3,
	first_hs_type = 4,
	hs_type_void = 4,
	hs_type_boolean = 5,
	hs_type_real = 6,
	hs_type_short_integer = 7,
	hs_type_long_integer = 8,
	hs_type_string = 9,
	hs_type_script = 10,
	hs_type_trigger_volume = 11,
	hs_type_cutscene_flag = 12,
	hs_type_cutscene_camera_point = 13,
	hs_type_cutscene_title = 14,
	hs_type_cutscene_recording = 15,
	hs_type_device_group = 16,
	hs_type_ai = 17,
	hs_type_ai_command_list = 18,
	hs_type_starting_profile = 19,
	hs_type_conversation = 20,
	hs_type_navpoint = 21,
	hs_type_hud_message = 22,
	hs_type_object_list = 23,
	first_hs_tag_reference_type = 24,
	hs_type_sound = 24,
	hs_type_effect = 25,
	hs_type_damage = 26,
	hs_type_looping_sound = 27,
	hs_type_animation_graph = 28,
	hs_type_actor_variant = 29,
	hs_type_damage_effect = 30,
	hs_type_object_definition = 31,
	not_last_hs_tag_reference_type = 32,
	number_of_hs_tag_reference_types = 8,
	last_hs_tag_reference_type = 31,
	first_hs_enum_type = 32,
	hs_type_enum_game_difficulty = 32,
	hs_type_enum_team = 33,
	hs_type_enum_ai_default_state = 34,
	hs_type_enum_actor_type = 35,
	hs_type_enum_hud_corner = 36,
	not_last_hs_enum_type = 37,
	number_of_hs_enum_types = 5,
	last_hs_enum_type = 36,
	first_hs_object_type = 37,
	hs_type_object = 37,
	hs_type_unit = 38,
	hs_type_vehicle = 39,
	hs_type_weapon = 40,
	hs_type_device = 41,
	hs_type_scenery = 42,
	first_hs_object_name_type = 43,
	last_hs_object_type = 42,
	hs_type_object_name = 43,
	number_of_hs_object_types = 6,
	last_hs_object_name_type = 48,
	number_of_hs_node_types = 49
} ;

#endif /* HS_TYPE_H */

// hs_type: DB-verified via types_enum_values _D0F641CA8E0315935795906396F6347C (anonymous compiled enum, names sans leading _)
