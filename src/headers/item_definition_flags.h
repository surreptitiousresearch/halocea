#ifndef ITEM_DEFINITION_FLAGS_H
#define ITEM_DEFINITION_FLAGS_H

/* item_definition_flags — item_definition flags.
 * DB-verified: types_enum_values _6C6BD5A867F7243F634F30F93C55A889 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum item_definition_flags {
	_item_always_maintains_z_up_bit = 0,
	_item_destroyed_by_explosions_bit = 1,
	_item_antigravity_bit = 2,
	NUMBER_OF_ITEM_DEFINITION_FLAGS = 3,
};

#endif /* ITEM_DEFINITION_FLAGS_H */

// item_definition_flags: DB-verified via types_enum_values _6C6BD5A867F7243F634F30F93C55A889
