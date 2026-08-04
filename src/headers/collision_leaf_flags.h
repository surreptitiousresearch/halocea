#ifndef COLLISION_LEAF_FLAGS_H
#define COLLISION_LEAF_FLAGS_H

/* collision_leaf_flags — collision_leaf flags.
 * DB-verified: types_enum_values _4B61485000CAECBE2045B38F863A5A86 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum collision_leaf_flags {
	_collision_leaf_contains_two_sided_bit = 0,
	NUMBER_OF_COLLISION_LEAF_FLAGS = 1,
};

#endif /* COLLISION_LEAF_FLAGS_H */

// collision_leaf_flags: DB-verified via types_enum_values _4B61485000CAECBE2045B38F863A5A86
