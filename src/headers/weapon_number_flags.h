#ifndef WEAPON_NUMBER_FLAGS_H
#define WEAPON_NUMBER_FLAGS_H

/* weapon_number_flags — weapon_number flags.
 * DB-verified: types_enum_values _54F206FD0338B4762FB824202AFDD540 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum weapon_number_flags {
	_weapon_number_divide_by_clip_size_bit = 0,
	NUMBER_OF_WEAPON_NUMBER_FLAGS = 1,
};

#endif /* WEAPON_NUMBER_FLAGS_H */

// weapon_number_flags: DB-verified via types_enum_values _54F206FD0338B4762FB824202AFDD540
