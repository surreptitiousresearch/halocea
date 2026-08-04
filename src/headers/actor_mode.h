#ifndef ACTOR_MODE_H
#define ACTOR_MODE_H

/* actor_mode - actor->state.mode top-level AI mode (the ">= _actor_mode_combat" in-combat threshold).
 * The 3-slot display name table @0x84184FBC is a lossy helper; these are the compiled names. */
enum actor_mode {
	_actor_mode_braindead = 0,
	_actor_mode_asleep = 1,
	_actor_mode_alert = 2,
	_actor_mode_combat = 3,
	NUMBER_OF_ACTOR_MODES = 4,
};

#endif /* ACTOR_MODE_H */

// actor_mode: DB-verified via types_enum_values _7D6E27E2D1C775EA8E41D0C603EF5733 (anonymous compiled enum, names sans leading _)
