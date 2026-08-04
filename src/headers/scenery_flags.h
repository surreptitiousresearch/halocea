#ifndef SCENERY_FLAGS_H
#define SCENERY_FLAGS_H

/* scenery_flags — scenery flags.
 * DB-verified: types_enum_values _17013C49C91C3CFBF413550A0EEBA85C carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum scenery_flags {
	_scenery_self_animated_bit = 0,
	NUMBER_OF_SCENERY_FLAGS = 1,
};

#endif /* SCENERY_FLAGS_H */

// scenery_flags: DB-verified via types_enum_values _17013C49C91C3CFBF413550A0EEBA85C
