#ifndef FIRST_PERSON_WEAPON_STATE_H
#define FIRST_PERSON_WEAPON_STATE_H

/* first_person_weapon_state — values of first_person_weapon.state (the local player's viewmodel
 * state machine).
 *
 * GUESS: enum recovered by reconciliation, no ground-truth source for the state NAMES themselves —
 * rationale: each state is named after the first-person animation it plays, per the 1:1 mapping in
 * first_person_animation_type_from_weapon_state @0x8369DF10 into the binary's own label table
 * first_person_weapon_animation_list_entries @0x84185E88 (see first_person_weapon_animation.h, which IS
 * ground truth). Cross-checks: first_person_weapon_next_state advances the shotgun per-shell chain
 * reload_empty/reload_full/enter -> exit_empty/exit_full exactly as those labels imply; put_away (0x12)
 * holds its final frame; overheating states drain into overheated. */
/* layout bound to DB enum: types_enum_values _3C0B343E4C3F62C32ACDEC490E0723CE */
enum first_person_weapon_state {
	_first_person_weapon_state_idle = 0x0,
	_first_person_weapon_state_overheating = 0x1,
	_first_person_weapon_state_overheating_again = 0x2,
	_first_person_weapon_state_overheated = 0x3,
	_first_person_weapon_state_charged = 0x4,
	_first_person_weapon_state_posing = 0x5,
	_first_person_weapon_state_primary_fire = 0x6,
	_first_person_weapon_state_secondary_fire = 0x7,
	_first_person_weapon_state_primary_misfire = 0x8,
	_first_person_weapon_state_secondary_misfire = 0x9,
	_first_person_weapon_state_melee = 0xA,
	_first_person_weapon_state_light_on = 0xB,
	_first_person_weapon_state_light_off = 0xC,
	_first_person_weapon_state_reload_while_empty = 0xD,
	_first_person_weapon_state_reload_while_full = 0xE,
	_first_person_weapon_state_shotgun_enter_reload = 0xF,
	_first_person_weapon_state_shotgun_exit_reload_empty = 0x10,
	_first_person_weapon_state_shotgun_exit_reload_full = 0x11,
	_first_person_weapon_state_put_away = 0x12,
	_first_person_weapon_state_ready = 0x13,
	_first_person_weapon_state_throw_grenade = 0x14,
	_first_person_weapon_state_throw_grenade_overheated = 0x15,
	_first_person_weapon_state_overheated_exit = 0x16,
	_first_person_weapon_state_overheating_super_recoil = 0x17,
	NUMBER_OF_FIRST_PERSON_WEAPON_STATES = 0x18,
};

#endif /* FIRST_PERSON_WEAPON_STATE_H */
