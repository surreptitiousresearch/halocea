#ifndef PROJECTILE_POSSIBLE_RESPONSE_FLAGS_H
#define PROJECTILE_POSSIBLE_RESPONSE_FLAGS_H

/* projectile_possible_response_flags — bit indices for the projectile_possible_response flags word.
 * DB-verified: types_enum_values _10924CABBEAB46B9AE2BA1AA15BBD878 carries these named bit
 * indices verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum projectile_possible_response_flags {
	_projectile_possible_response_only_against_units_bit = 0,
	NUMBER_OF_PROJECTILE_POSSIBLE_RESPONSE_FLAGS = 1,
};

#endif /* PROJECTILE_POSSIBLE_RESPONSE_FLAGS_H */

// projectile_possible_response_flags: DB-verified via types_enum_values _10924CABBEAB46B9AE2BA1AA15BBD878
