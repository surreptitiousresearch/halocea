#ifndef CONTRAIL_ACTIVE_FLAGS_H
#define CONTRAIL_ACTIVE_FLAGS_H

/* contrail_active_flags - bit indices for the contrail_active.flags word.
 * DB-verified: types_enum_values _92AA4B8BF4A7D1621514205B5EE9F89B carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum contrail_active_flags {
	_contrail_active_bit = 0,
	NUMBER_OF_CONTRAIL_FLAGS = 1,
};

#endif /* CONTRAIL_ACTIVE_FLAGS_H */

// contrail_active_flags: DB-verified via types_enum_values _92AA4B8BF4A7D1621514205B5EE9F89B
