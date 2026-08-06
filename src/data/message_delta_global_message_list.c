/* message_delta_global_message_list @ 0x82129A10 (.rdata, 188 bytes)
 * DB applied_types: _message_definition *const message_delta_global_message_list[47];
 * Image bytes (big-endian), decoded from the binary .rdata record:
 *   +0x0000 = 0x841767D0 -> message_object_deletion_definition
 *   +0x0004 = 0x8417EAF8 -> message_projectile_update_definition
 *   +0x0008 = 0x84184A58 -> message_equipment_update_definition
 *   +0x000C = 0x84176430 -> message_weapon_update_definition
 *   +0x0010 = 0x841848E0 -> message_biped_update_definition
 *   +0x0014 = 0x8417EC88 -> message_vehicle_update_definition
 *   +0x0018 = 0x84175918 -> message_hud_add_item_definition
 *   +0x001C = 0x84175A70 -> message_player_create_definition
 *   +0x0020 = 0x84175AE8 -> message_player_spawn_definition
 *   +0x0024 = 0x841760F8 -> message_player_exit_vehicle_definition
 *   +0x0028 = 0x84175BE8 -> message_player_set_action_result_definition
 *   +0x002C = 0x8417C9D0 -> message_player_effect_start_definition
 *   +0x0030 = 0x84176000 -> message_unit_kill_definition
 *   +0x0034 = 0x8417DD60 -> message_client_game_update_definition
 *   +0x0038 = 0x84175CA8 -> message_player_handle_powerup_definition
 *   +0x003C = 0x841858D8 -> message_slayer_update_definition
 *   +0x0040 = 0x84185548 -> message_ctf_update_definition
 *   +0x0044 = 0x84185AB8 -> message_oddball_update_definition
 *   +0x0048 = 0x84186C00 -> message_king_update_definition
 *   +0x004C = 0x84186A68 -> message_race_update_definition
 *   +0x0050 = 0x8417DA00 -> message_player_score_update_definition
 *   +0x0054 = 0x8417DB48 -> message_game_engine_change_mode_definition
 *   +0x0058 = 0x8417D948 -> message_multiplayer_hud_message_definition
 *   +0x005C = 0x84184B78 -> message_multiplayer_sound_definition
 *   +0x0060 = 0x841853B0 -> message_team_change_definition
 *   +0x0064 = 0x84176150 -> message_unit_drop_current_weapon_definition
 *   +0x0068 = 0x8417EB60 -> message_vehicle_new_definition
 *   +0x006C = 0x84184750 -> message_biped_new_definition
 *   +0x0070 = 0x8417EA10 -> message_projectile_new_definition
 *   +0x0074 = 0x84184960 -> message_equipment_new_definition
 *   +0x0078 = 0x841762F8 -> message_weapon_new_definition
 *   +0x007C = 0x841819B8 -> message_game_settings_update_definition
 *   +0x0080 = 0x841856D8 -> message_parameters_protocol_definition
 *   +0x0084 = 0x8417DE28 -> message_local_player_update_definition
 *   +0x0088 = 0x8417DE90 -> message_local_player_vehicle_update_definition
 *   +0x008C = 0x8417DF90 -> message_remote_player_action_update_definition
 *   +0x0090 = 0x8417E038 -> message_super_remote_players_action_update_definition
 *   +0x0094 = 0x8417E128 -> message_remote_player_position_update_definition
 *   +0x0098 = 0x8417E170 -> message_remote_player_vehicle_update_definition
 *   +0x009C = 0x84176278 -> message_weapon_start_reload_definition
 *   ... 28 further bytes elided; full hex in .sweep/data_image.tsv
 * the
 * message-delta processor's dispatch table: message definition object for each
 * message_delta_processor_message_definition_type id. Both the encoder and the decoder reach a
 * message's field-reference sets only through this table (encode_iterations @0x837A26B8 and
 * decode_iteration_independent_header @0x837A1F20 both index it with the raw definition_type),
 * and initialize_messages/dispose_messages walk it linearly at startup and shutdown.
 * Extent and element type are PROVEN, not inferred. The DB applied type at the symbol address reads
 * `_message_definition *const message_delta_global_message_list[47]`, the preceding .rdata object is the
 * string "message metrics" ending at 0x82129A0F and the next name is __real@bd036d41 at 0x82129ACC, so the
 * array is exactly 47 words wide. initialize_messages @0x837A2B28 forms the base with
 * `lis r11, ...@ha / addi r26, r11, ...@l` (raw 0x3D408213 / 0x38EA9A10 -> 0x82129A10, no pointer load) and
 * ends its walk at `addi r5, r26, 0xBC` = base + 47 * 4. message_definition_initialize @0x837A2A50 indexes
 * it with `slwi r6, r11, 2 / lwzx r5, r6, r7` off the same base.
 * Slot 46 is a NULL terminator; slots 0..45 are the 46 named .data _message_definition objects, each
 * authored in its own src/data TU.
 * ORIGINAL-BINARY ANOMALY (preserved verbatim, do not "fix"): the table is one entry short of the DB enum
 * message_delta_processor_message_definition_type. `_message_hud_chat` (15) owns no message definition
 * object, so slot 15 already holds `_message_slayer_update` (16) and every id from 16 up sits one slot below
 * its enum value. The binary nonetheless passes enum values as definition_type — verified structurally, not
 * just numerically: 0x8374D730 passes 0x17 (_message_multiplayer_hud_message = 23) after building a
 * three-word source struct whose word 0 comes from field_translated_index_translate_index against
 * field_properties_player_index_definition, which matches slot 22 (player_index/integer_large/integer_large)
 * and not slot 23 (a single integer_large); and 0x83759888 passes 0x2E (_message_projectile_attach = 46)
 * after building translated-index/translated-index/int16_t, which matches slot 45. So
 * message_delta_global_message_list[definition_type] selects the *next* message type's field-reference set
 * for every id >= 16, and for id 46 it selects the NULL terminator, which encode_iterations and
 * decode_iteration_independent_header dereference immediately (the latter's bound check is
 * `definition_type <= _message_projectile_attach`, i.e. 46 is accepted). The path never executes in this
 * build: message_delta_processor_initialize @0x837A2C10 has no code xref at all — its only reference is a
 * .pdata unwind record — so the legacy Blam message-delta processor is dead code in HCEX.
 * QUALIFIER (adopted 2026-08-06, BACKLOG A6c): the elements are `_message_definition *const`, as the DB
 * applied type states, and that const is load-bearing rather than cosmetic. C11 6.2.5p26 makes an array
 * and its element type identically qualified, so `T *const a[47]` is a const-qualified object and MSVC
 * emits it into `.rdata`, while the unqualified `T *a[47]` goes to `.data` — measured, not argued:
 * compiling both spellings of this exact declaration and dumping the COFF section table gives SECTION
 * HEADER #3 `.rdata` for the const form and `.data` for the plain one. This symbol is at 0x82129A10, inside
 * `.rdata` (0x82000400..0x822CF970), while all 46 _message_definition objects it points at are `.data` — so
 * within one family the const spelling is exactly the one whose section matches the image. Dropping it would
 * have put this object in the wrong section of a rebuilt Blam .lib. Nothing writes through the table (all 25
 * consumers only read `list[i]` and then dereference, and the pointee stays non-const, which is what
 * message_definition_initialize mutates), so the qualifier costs no consumer anything.
 */
#include "../headers/message_definition.h"

extern _message_definition message_object_deletion_definition;                      /* 0x841767D0 */
extern _message_definition message_projectile_update_definition;                    /* 0x8417EAF8 */
extern _message_definition message_equipment_update_definition;                     /* 0x84184A58 */
extern _message_definition message_weapon_update_definition;                        /* 0x84176430 */
extern _message_definition message_biped_update_definition;                         /* 0x841848E0 */
extern _message_definition message_vehicle_update_definition;                       /* 0x8417EC88 */
extern _message_definition message_hud_add_item_definition;                         /* 0x84175918 */
extern _message_definition message_player_create_definition;                        /* 0x84175A70 */
extern _message_definition message_player_spawn_definition;                         /* 0x84175AE8 */
extern _message_definition message_player_exit_vehicle_definition;                  /* 0x841760F8 */
extern _message_definition message_player_set_action_result_definition;             /* 0x84175BE8 */
extern _message_definition message_player_effect_start_definition;                  /* 0x8417C9D0 */
extern _message_definition message_unit_kill_definition;                            /* 0x84176000 */
extern _message_definition message_client_game_update_definition;                   /* 0x8417DD60 */
extern _message_definition message_player_handle_powerup_definition;                /* 0x84175CA8 */
extern _message_definition message_slayer_update_definition;                        /* 0x841858D8 */
extern _message_definition message_ctf_update_definition;                           /* 0x84185548 */
extern _message_definition message_oddball_update_definition;                       /* 0x84185AB8 */
extern _message_definition message_king_update_definition;                          /* 0x84186C00 */
extern _message_definition message_race_update_definition;                          /* 0x84186A68 */
extern _message_definition message_player_score_update_definition;                  /* 0x8417DA00 */
extern _message_definition message_game_engine_change_mode_definition;              /* 0x8417DB48 */
extern _message_definition message_multiplayer_hud_message_definition;              /* 0x8417D948 */
extern _message_definition message_multiplayer_sound_definition;                    /* 0x84184B78 */
extern _message_definition message_team_change_definition;                          /* 0x841853B0 */
extern _message_definition message_unit_drop_current_weapon_definition;             /* 0x84176150 */
extern _message_definition message_vehicle_new_definition;                          /* 0x8417EB60 */
extern _message_definition message_biped_new_definition;                            /* 0x84184750 */
extern _message_definition message_projectile_new_definition;                       /* 0x8417EA10 */
extern _message_definition message_equipment_new_definition;                        /* 0x84184960 */
extern _message_definition message_weapon_new_definition;                           /* 0x841762F8 */
extern _message_definition message_game_settings_update_definition;                 /* 0x841819B8 */
extern _message_definition message_parameters_protocol_definition;                  /* 0x841856D8 */
extern _message_definition message_local_player_update_definition;                  /* 0x8417DE28 */
extern _message_definition message_local_player_vehicle_update_definition;          /* 0x8417DE90 */
extern _message_definition message_remote_player_action_update_definition;          /* 0x8417DF90 */
extern _message_definition message_super_remote_players_action_update_definition;   /* 0x8417E038 */
extern _message_definition message_remote_player_position_update_definition;        /* 0x8417E128 */
extern _message_definition message_remote_player_vehicle_update_definition;         /* 0x8417E170 */
extern _message_definition message_weapon_start_reload_definition;                  /* 0x84176278 */
extern _message_definition message_weapon_ammo_pickup_mid_reload_definition;        /* 0x84176200 */
extern _message_definition message_netgame_equipment_new_definition;                /* 0x8417D8E0 */
extern _message_definition message_projectile_detonate_definition;                  /* 0x8417E950 */
extern _message_definition message_item_accelerate_definition;                      /* 0x84175D88 */
extern _message_definition message_damage_dealt_definition;                         /* 0x84175D20 */
extern _message_definition message_projectile_attach_definition;                    /* 0x8417E9A8 */

_message_definition *const message_delta_global_message_list[47] =
{
    &message_object_deletion_definition,                      /* [ 0] 0x841767D0 -> _message_object_deletion */
    &message_projectile_update_definition,                    /* [ 1] 0x8417EAF8 -> _message_projectile_update */
    &message_equipment_update_definition,                     /* [ 2] 0x84184A58 -> _message_equipment_update */
    &message_weapon_update_definition,                        /* [ 3] 0x84176430 -> _message_weapon_update */
    &message_biped_update_definition,                         /* [ 4] 0x841848E0 -> _message_biped_update */
    &message_vehicle_update_definition,                       /* [ 5] 0x8417EC88 -> _message_vehicle_update */
    &message_hud_add_item_definition,                         /* [ 6] 0x84175918 -> _message_hud_add_item */
    &message_player_create_definition,                        /* [ 7] 0x84175A70 -> _message_player_create */
    &message_player_spawn_definition,                         /* [ 8] 0x84175AE8 -> _message_player_spawn */
    &message_player_exit_vehicle_definition,                  /* [ 9] 0x841760F8 -> _message_player_exit_vehicle */
    &message_player_set_action_result_definition,             /* [10] 0x84175BE8 -> _message_player_set_action_result */
    &message_player_effect_start_definition,                  /* [11] 0x8417C9D0 -> _message_player_effect_start */
    &message_unit_kill_definition,                            /* [12] 0x84176000 -> _message_unit_kill */
    &message_client_game_update_definition,                   /* [13] 0x8417DD60 -> _message_client_game_update */
    &message_player_handle_powerup_definition,                /* [14] 0x84175CA8 -> _message_player_handle_powerup */
    &message_slayer_update_definition,                        /* [15] 0x841858D8 -> _message_slayer_update */
    &message_ctf_update_definition,                           /* [16] 0x84185548 -> _message_ctf_update */
    &message_oddball_update_definition,                       /* [17] 0x84185AB8 -> _message_oddball_update */
    &message_king_update_definition,                          /* [18] 0x84186C00 -> _message_king_update */
    &message_race_update_definition,                          /* [19] 0x84186A68 -> _message_race_update */
    &message_player_score_update_definition,                  /* [20] 0x8417DA00 -> _message_player_score_update */
    &message_game_engine_change_mode_definition,              /* [21] 0x8417DB48 -> _message_game_engine_change_mode */
    &message_multiplayer_hud_message_definition,              /* [22] 0x8417D948 -> _message_multiplayer_hud_message */
    &message_multiplayer_sound_definition,                    /* [23] 0x84184B78 -> _message_multiplayer_sound */
    &message_team_change_definition,                          /* [24] 0x841853B0 -> _message_team_change */
    &message_unit_drop_current_weapon_definition,             /* [25] 0x84176150 -> _message_unit_drop_current_weapon */
    &message_vehicle_new_definition,                          /* [26] 0x8417EB60 -> _message_vehicle_new */
    &message_biped_new_definition,                            /* [27] 0x84184750 -> _message_biped_new */
    &message_projectile_new_definition,                       /* [28] 0x8417EA10 -> _message_projectile_new */
    &message_equipment_new_definition,                        /* [29] 0x84184960 -> _message_equipment_new */
    &message_weapon_new_definition,                           /* [30] 0x841762F8 -> _message_weapon_new */
    &message_game_settings_update_definition,                 /* [31] 0x841819B8 -> _message_game_settings_update */
    &message_parameters_protocol_definition,                  /* [32] 0x841856D8 -> _message_parameters_protocol */
    &message_local_player_update_definition,                  /* [33] 0x8417DE28 -> _message_local_player_update */
    &message_local_player_vehicle_update_definition,          /* [34] 0x8417DE90 -> _message_local_player_vehicle_update */
    &message_remote_player_action_update_definition,          /* [35] 0x8417DF90 -> _message_remote_player_action_update */
    &message_super_remote_players_action_update_definition,   /* [36] 0x8417E038 -> _message_super_remote_players_action_update */
    &message_remote_player_position_update_definition,        /* [37] 0x8417E128 -> _message_remote_player_position_update */
    &message_remote_player_vehicle_update_definition,         /* [38] 0x8417E170 -> _message_remote_player_vehicle_update */
    &message_weapon_start_reload_definition,                  /* [39] 0x84176278 -> _message_weapon_start_reload */
    &message_weapon_ammo_pickup_mid_reload_definition,        /* [40] 0x84176200 -> _message_weapon_ammo_pickup_mid_reload */
    &message_netgame_equipment_new_definition,                /* [41] 0x8417D8E0 -> _message_netgame_equipment_new */
    &message_projectile_detonate_definition,                  /* [42] 0x8417E950 -> _message_projectile_detonate */
    &message_item_accelerate_definition,                      /* [43] 0x84175D88 -> _message_item_accelerate */
    &message_damage_dealt_definition,                         /* [44] 0x84175D20 -> _message_damage_dealt */
    &message_projectile_attach_definition,                    /* [45] 0x8417E9A8 -> _message_projectile_attach */
    0,                                                        /* [46] 0x00000000 NULL terminator */
};
