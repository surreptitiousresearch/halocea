#ifndef CREDITS_ENDGAME_MODE_H
#define CREDITS_ENDGAME_MODE_H

/* End-of-campaign credits roll state (credits_post_render / credits update). */
enum credits_endgame_mode {
	CREDITS_NOT_ENDGAME = 0,
	CREDITS_ENDGAME_ROLLING = 1,
	CREDITS_ENDGAME_FINISHED = 2,
};

#endif /* CREDITS_ENDGAME_MODE_H */

// credits_endgame_mode: DB-verified via types_enum_values $7E1BF51E1D6B82710686382CEFFB7C60
