#ifndef SQUAD_FLAGS_H
#define SQUAD_FLAGS_H

/* squad_flags — squad flags.
 * DB-verified: types_enum_values _482A8082237B51B1CD35BAC2B6937C10 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum squad_flags {
	_squad_unused_bit = 0,
	_squad_never_search_bit = 1,
	_squad_timer_starts_immediately_bit = 2,
	_squad_delay_forever_bit = 3,
	_squad_magic_sight_after_timer_bit = 4,
	_squad_automatic_migration_bit = 5,
	NUMBER_OF_SQUAD_FLAGS = 6,
};

#endif /* SQUAD_FLAGS_H */

// squad_flags: DB-verified via types_enum_values _482A8082237B51B1CD35BAC2B6937C10
