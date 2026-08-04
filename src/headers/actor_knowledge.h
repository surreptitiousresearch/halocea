#ifndef ACTOR_KNOWLEDGE_H
#define ACTOR_KNOWLEDGE_H

/* actor_knowledge — how much "combat knowledge" an actor has about a target (0..3), used to scale
 * perception/visibility tests (actor_get_perception_knowledge return value).
 * DB-verified via types_enum_values _A1C8AE3AB449E442CE82FBD4A438E7DF (anonymous compiled enum). */
enum actor_knowledge {
	_actor_knowledge_noncombat = 0,
	_actor_knowledge_guard = 1,
	_actor_knowledge_searching = 2,
	_actor_knowledge_definite = 3,
	NUMBER_OF_ACTOR_KNOWLEDGE_TYPES = 4,
};

#endif /* ACTOR_KNOWLEDGE_H */
