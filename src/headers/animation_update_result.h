#pragma once

/* animation_update_result — the code animation_update_internal (@0x837969A0) returns, and the domain
 * its callers test: first_person_weapon_update, scenery_update, weapon_update and unit_update_animation
 * all compare the result against 1, 2 and 4. The callee's own body fixes the meanings — it returns the
 * key-frame test (0/1) while still inside the clip, 2 on the last-but-one frame of a non-looping clip,
 * 3 after choosing a random next permutation and restarting, and 4 after jumping to the loop frame.
 * Source: types_enum_values $096E6F8396F9460DDB0339A7BCA35BED (DB-verified). The DB enum is anonymous;
 * the tag name comes from its own NUMBER_OF_ANIMATION_UPDATE_RESULTS member rather than from the bare
 * member prefix, because `animation` is already a struct tag (animation.h) and C keeps struct and enum
 * tags in one namespace. */
enum animation_update_result {
	_animation_running = 0,
	_animation_key_frame = 1,
	_animation_will_restart_on_next_frame = 2,
	_animation_restarted = 3,
	_animation_looped = 4,
	NUMBER_OF_ANIMATION_UPDATE_RESULTS = 5,
};

// animation_update_result: DB-verified via types_enum_values $096E6F8396F9460DDB0339A7BCA35BED
