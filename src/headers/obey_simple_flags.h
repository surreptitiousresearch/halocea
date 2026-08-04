#ifndef OBEY_SIMPLE_FLAGS_H
#define OBEY_SIMPLE_FLAGS_H

/* obey_simple_flags — obey_simple flags.
 * DB-verified: types_enum_values _5F14168CA5C13897CF031FE97B7BC729 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum obey_simple_flags {
	_obey_simple_directmovement_bit = 0,
	_obey_simple_directmovement_update_continuously_bit = 1,
	_obey_simple_jump_bit = 2,
	_obey_simple_jump_jumped_bit = 3,
	_obey_simple_jump_targeted_bit = 4,
	NUMBER_OF_OBEY_SIMPLE_FLAGS = 5,
};

#endif /* OBEY_SIMPLE_FLAGS_H */

// obey_simple_flags: DB-verified via types_enum_values _5F14168CA5C13897CF031FE97B7BC729
