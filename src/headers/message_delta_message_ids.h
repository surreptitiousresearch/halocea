#pragma once
/* message_delta_message_ids — message_delta_processor_message_definition_type id constants (indices into
 * message_delta_global_message_list). Every id that owns a message definition object is listed, plus
 * _message_hud_chat, which owns none.
 *
 * Every value below is the DB enum message_delta_processor_message_definition_type, cross-checked against the
 * literal the binary passes as `definition_type` wherever a call site exists (the `disasm` notes). The two
 * oracles agree on all 24 cross-checked ids.
 *
 * The DB enum runs 0..50; ids 47..50 (_message_add_player_ingame, _message_remove_player_ingame,
 * _message_game_over, _message_client_graceful_exit_ingame) own no message definition object and are not
 * listed here, and neither is _message_hud_chat's definition — see the note on _message_hud_chat below. */

enum
{
    _message_object_deletion = 0, /* disasm 0x836EBD38: li r3, 0 # definition_type */
    _message_projectile_update = 1,
    _message_equipment_update = 2,
    _message_weapon_update = 3,
    _message_biped_update = 4,
    _message_vehicle_update = 5,
    _message_hud_add_item = 6, /* disasm 0x836A5C64: li r3, 6 # definition_type */
    _message_player_create = 7,
    _message_player_spawn = 8,
    _message_player_exit_vehicle = 9,
    _message_player_set_action_result = 10, /* disasm 0x836A99D8: li r3, 0xA # definition_type */
    _message_player_effect_start = 11, /* DB message_delta_processor_message_definition_type value 11 */
    _message_unit_kill = 12,
    _message_client_game_update = 13,
    _message_player_handle_powerup = 14, /* disasm 0x836AA430: li r3, 0xE # definition_type */
    /* _message_hud_chat = 15 is in the DB enum but owns no message definition object and has no slot in
     * message_delta_global_message_list, which is why that table is 47 entries (46 definitions plus a NULL
     * terminator) rather than 48. Every id from 16 up therefore sits one slot lower in the table than its
     * enum value — see src/data/message_delta_global_message_list.c. */
    _message_hud_chat = 15,
    _message_slayer_update = 16,
    _message_ctf_update = 17,
    _message_oddball_update = 18, /* DB message_delta_processor_message_definition_type value 18 */
    _message_king_update = 19,
    _message_race_update = 20,
    _message_player_score_update = 21,
    _message_game_engine_change_mode = 22, /* disasm: li r3, 0x16 # definition_type */
    _message_multiplayer_hud_message = 23, /* disasm 0x8374D730: li r3, 0x17 # definition_type */
    _message_multiplayer_sound = 24, /* disasm 0x837B6914: li r3, 0x18 # definition_type */
    _message_team_change = 25, /* disasm 0x838029D8: li r3, 0x19 # definition_type */
    _message_unit_drop_current_weapon = 26, /* disasm: li r3, 0x1A # definition_type */
    _message_vehicle_new = 27,
    _message_biped_new = 28, /* disasm 0x837AD8C8: li r3, 0x1C # definition_type */
    _message_projectile_new = 29, /* disasm 0x8375A650: li r3, 0x1D # definition_type */
    _message_equipment_new = 30,
    _message_weapon_new = 31,
    _message_game_settings_update = 32,
    _message_parameters_protocol = 33, /* disasm 0x83812248: li r3, 0x21 # definition_type */
    _message_local_player_update = 34, /* disasm 0x83756FD4: li r3, 0x22 # definition_type */
    _message_local_player_vehicle_update = 35, /* disasm 0x83757198: li r3, 0x23 # definition_type */
    _message_remote_player_action_update = 36, /* disasm 0x83757AD8: li r3, 0x24 # definition_type */
    _message_super_remote_players_action_update = 37, /* disasm 0x83756ED0: li r3, 0x25 # definition_type */
    _message_remote_player_position_update = 38, /* disasm 0x837572C8: li r3, 0x26 # definition_type */
    _message_remote_player_vehicle_update = 39, /* disasm 0x837574C4: li r3, 0x27 # definition_type */
    _message_weapon_start_reload = 40, /* disasm 0x836D9AD0: li r3, 0x28 # definition_type */
    _message_weapon_ammo_pickup_mid_reload = 41,
    _message_netgame_equipment_new = 42,
    _message_projectile_detonate = 43,
    _message_item_accelerate = 0x2C, /* headers_ref message_delta_processor_message_definition_type */
    _message_damage_dealt = 0x2D, /* headers_ref message_delta_processor_message_definition_type */
    _message_projectile_attach = 0x2E, /* disasm 0x83759888: li r3, 0x2E — highest type accepted by the iterated decode path */
};
