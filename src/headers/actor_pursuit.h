#pragma once

/* actor_pursuit — the per-actor-type "when to do it" setting for the lost-contact pursuit
 * options: actor_type_definition.when_to_search_at_target / when_to_pursue /
 * when_to_search_pursuit, plus the matching desired_* out-parameters of
 * encounter_modify_pursuit_desires @0x83709EA0. actor_action_determine_pursuit_options
 * @0x837F2A60 is the decisive consumer: value 2 forecloses the option outright, while
 * value 1 forecloses it only when this actor is the controlling group-pursuit leader
 * (`desire == 1 && controlling_group_pursuit`) — literally never / not-when-group — and
 * 0 leaves it permitted (always). actor_action_handle_lost_contact @0x837F5C38 carries the
 * three values from the actor-type getters into those argument slots.
 * Source: types_enum_values $50834E6E067871812724FE8B8E34B780 (DB-verified).
 * Not to be confused with group_pursuit_restriction ($FCE094907EBC11723C85F63A8080CEED),
 * the separate 0/1/2 domain of this function's group_pursuit_restriction argument. */
enum actor_pursuit {
	_actor_pursuit_always = 0,
	_actor_pursuit_not_when_group = 1,
	_actor_pursuit_never = 2,
	NUMBER_OF_ACTOR_PURSUIT_SETTINGS = 3,
};

// actor_pursuit: DB-verified via types_enum_values $50834E6E067871812724FE8B8E34B780
