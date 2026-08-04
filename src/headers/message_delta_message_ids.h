#pragma once
/* message_delta_message_ids — message_delta_processor_message_definition_type id constants (indices into
 * message_delta_global_message_list). Only the ids referenced by reconstructed code are listed. */

enum
{
    _message_player_create = 7,
    _message_player_spawn = 8,
    _message_player_set_action_result = 10, /* disasm 0x836A99D8: li r3, 0xA # definition_type */
    _message_hud_add_item = 6, /* disasm 0x836A5C64: li r3, 6 # definition_type */
    _message_game_engine_change_mode = 22, /* disasm: li r3, 0x16 # definition_type */
    _message_unit_drop_current_weapon = 26, /* disasm: li r3, 0x1A # definition_type */
    _message_multiplayer_hud_message = 23, /* disasm 0x8374D730: li r3, 0x17 # definition_type */
    _message_player_effect_start = 11, /* DB message_delta_processor_message_definition_type value 11 */
    _message_slayer_update = 16,
    _message_ctf_update = 17,
    _message_oddball_update = 18, /* DB message_delta_processor_message_definition_type value 18 */
    _message_equipment_new = 30,
    _message_weapon_new = 31,
    _message_netgame_equipment_new = 42,
    _message_king_update = 19,
    _message_race_update = 20,
    _message_projectile_detonate = 43,
    _message_player_score_update = 21,
    _message_game_settings_update = 32,
    _message_weapon_ammo_pickup_mid_reload = 41,
    _message_vehicle_new = 27,
    _message_item_accelerate = 0x2C, /* headers_ref message_delta_processor_message_definition_type */
    _message_damage_dealt = 0x2D, /* headers_ref message_delta_processor_message_definition_type */
    _message_projectile_attach = 0x2E, /* DB message_delta_processor_message_definition_type value 46 — highest type accepted by the iterated decode path */
};
