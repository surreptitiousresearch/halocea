/* DB-verified 2026-07-20 (BLOCKED-retry): the numeric order matches compiled enum types_enum_values
 * _0F585B2A754502D879CE7D3548003A9B (DB uses the _unit_weapon_class_animation_* prefix; the
 * _weapon_class_animation_* identifiers below are the consumer-facing names and are kept). DB gives the
 * authoritative labels for a few reconstructed slots: 19 = land_mine (local "unused19"),
 * 34 = signal_warn (local "warn"), 35-38 = moving_wounded_* (local "stunned_*"). */
#ifndef WEAPON_CLASS_ANIMATION_H
#define WEAPON_CLASS_ANIMATION_H

/* weapon_class_animation — index into an animation_graph_weapon_class's animations block
 * (the per-weapon-class unit animation table).
 * Source: binary label table weapon_class_animation_list_entries @0x84185B80 (55 entries of
 * {char *label, 0}); labels read directly from .data (entries 0/22/34 share string storage with other
 * tables and were byte-read: "idle"/"drop"/"warn"), entry order is the enum value. Ground truth. */
enum weapon_class_animation {
	_weapon_class_animation_idle = 0,
	_weapon_class_animation_gesture = 1,
	_weapon_class_animation_turn_left = 2,
	_weapon_class_animation_turn_right = 3,
	_weapon_class_animation_dive_front = 4,
	_weapon_class_animation_dive_back = 5,
	_weapon_class_animation_dive_left = 6,
	_weapon_class_animation_dive_right = 7,
	_weapon_class_animation_move_front = 8,
	_weapon_class_animation_move_back = 9,
	_weapon_class_animation_move_left = 10,
	_weapon_class_animation_move_right = 11,
	_weapon_class_animation_slide_front = 12,
	_weapon_class_animation_slide_back = 13,
	_weapon_class_animation_slide_left = 14,
	_weapon_class_animation_slide_right = 15,
	_weapon_class_animation_airborne = 16,
	_weapon_class_animation_land_soft = 17,
	_weapon_class_animation_land_hard = 18,
	_weapon_class_animation_unused19 = 19,        /* "unused" */
	_weapon_class_animation_throw_grenade = 20,
	_weapon_class_animation_disarm = 21,
	_weapon_class_animation_drop = 22,
	_weapon_class_animation_ready = 23,
	_weapon_class_animation_put_away = 24,
	_weapon_class_animation_aim_still = 25,
	_weapon_class_animation_aim_move = 26,
	_weapon_class_animation_surprise_front = 27,
	_weapon_class_animation_surprise_back = 28,
	_weapon_class_animation_berserk = 29,
	_weapon_class_animation_evade_left = 30,
	_weapon_class_animation_evade_right = 31,
	_weapon_class_animation_signal_move = 32,
	_weapon_class_animation_signal_attack = 33,
	_weapon_class_animation_warn = 34,
	_weapon_class_animation_stunned_front = 35,
	_weapon_class_animation_stunned_back = 36,
	_weapon_class_animation_stunned_left = 37,
	_weapon_class_animation_stunned_right = 38,
	_weapon_class_animation_melee = 39,
	_weapon_class_animation_celebrate = 40,
	_weapon_class_animation_panic = 41,
	_weapon_class_animation_melee_airborne = 42,
	_weapon_class_animation_flaming = 43,
	_weapon_class_animation_resurrect_front = 44,
	_weapon_class_animation_resurrect_back = 45,
	_weapon_class_animation_melee_continuous = 46,
	_weapon_class_animation_feeding = 47,
	_weapon_class_animation_leap_start = 48,
	_weapon_class_animation_leap_airborne = 49,
	_weapon_class_animation_leap_melee = 50,
	_weapon_class_animation_zapping = 51,
	_weapon_class_animation_unused52 = 52,        /* "unused" */
	_weapon_class_animation_unused53 = 53,        /* "unused" */
	_weapon_class_animation_unused54 = 54,        /* "unused" */
	NUMBER_OF_WEAPON_CLASS_ANIMATIONS = 55,
};

#endif /* WEAPON_CLASS_ANIMATION_H */
