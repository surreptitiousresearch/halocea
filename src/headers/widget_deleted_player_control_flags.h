#pragma once

/* widget_deleted_player_control_flags — the mask of ALL player_control_action_test bits (4095 = bits 0..11), applied when a widget is deleted — see src/headers/player_control_action_test_flags.h for the per-bit family.
 * Source: types_enum_values $CECF2349DBAA0A3CAE589105DFC519EE (DB-verified, members verbatim in DB order). */
enum widget_deleted_player_control_flags {
	WIDGET_DELETED_PLAYER_CONTROL_INHIBIT_FLAGS = 4095,
};

// widget_deleted_player_control_flags: DB-verified via types_enum_values $CECF2349DBAA0A3CAE589105DFC519EE
