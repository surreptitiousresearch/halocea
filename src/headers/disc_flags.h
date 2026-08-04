#ifndef DISC_FLAGS_H
#define DISC_FLAGS_H

/* disc_flags — disc flags.
 * DB-verified: types_enum_values _39B4237A37C3596AE37229CB94EB4F6E carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum disc_flags {
	_disc_optional_bit = 0,
	NUMBER_OF_DISC_FLAGS = 1,
};

#endif /* DISC_FLAGS_H */

// disc_flags: DB-verified via types_enum_values _39B4237A37C3596AE37229CB94EB4F6E
