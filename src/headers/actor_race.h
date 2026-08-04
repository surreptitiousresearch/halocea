#pragma once
/* actor_race — AI race bitmask (and its bit indices) used by perception/communication filters and
 * ai_get_race_from_team_index.c. Source: headers_ref _AFFFB5C6624ED582F1B8FE627CC7C4A1. */

/* The DB models both the bit indices and the bit-mask values as a SINGLE anonymous
 * enum (_AFFFB5C6624ED582F1B8FE627CC7C4A1), in this order. Reproduced verbatim. */
enum actor_race {
	_race_player_bit = 0,
	_race_human_bit = 1,
	_race_covenant_bit = 2,
	_race_floodcombat_bit = 3,
	_race_floodcarrier_bit = 4,
	_race_floodinfection_bit = 5,
	_race_sentinel_bit = 6,
	NUMBER_OF_ACTOR_RACE_FLAGS = 7,
	_race_none = 0x0,
	_race_player = 0x1,
	_race_human = 0x2,
	_race_covenant = 0x4,
	_race_floodcombat = 0x8,
	_race_floodcarrier = 0x10,
	_race_floodinfection = 0x20,
	_race_flood = 0x38,
	_race_sentinel = 0x40,
	_race_all = 0x7F,
};

// actor_race: DB-verified via types_enum_values _AFFFB5C6624ED582F1B8FE627CC7C4A1 (anonymous compiled enum, names sans leading _)
