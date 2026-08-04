#ifndef ACTOR_POSTCOMBAT_TYPE_H
#define ACTOR_POSTCOMBAT_TYPE_H

enum actor_postcombat_type {
	actor_postcombat_none = 0,
	actor_postcombat_speak_alone = 1,
	actor_postcombat_speak_unscathed = 2,
	actor_postcombat_speak_wounded = 3,
	actor_postcombat_speak_massacre = 4,
	actor_postcombat_speak_triumph = 5,
	actor_postcombat_run_to = 6,
	actor_postcombat_check_enemy = 7,
	actor_postcombat_check_friend = 8,
	actor_postcombat_shoot_corpse = 9,
	actor_postcombat_celebrate = 10,
	number_of_actor_postcombat_actions = 11,
};

#endif /* ACTOR_POSTCOMBAT_TYPE_H */

// actor_postcombat_type: DB-verified via types_enum_values _AC0AC5D3006E12ACF0D816C905520723 (anonymous compiled enum, names sans leading _)
