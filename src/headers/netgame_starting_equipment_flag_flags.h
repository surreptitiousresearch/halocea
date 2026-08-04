#ifndef NETGAME_STARTING_EQUIPMENT_FLAG_FLAGS_H
#define NETGAME_STARTING_EQUIPMENT_FLAG_FLAGS_H

/* netgame_starting_equipment_flag_flags - bit indices for the netgame_starting_equipment_flag.flags word.
 * DB-verified: types_enum_values _A97CE003292EA707A93C4DE87774387F carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum netgame_starting_equipment_flag_flags {
	_netgame_starting_equipment_flag_no_grenades_bit = 0,
	_netgame_starting_equipment_flag_plasma_greandes_bit = 1,
	NUMBER_OF_NETGAME_STARTING_EQUIPMENT_FLAGS = 2,
};

#endif /* NETGAME_STARTING_EQUIPMENT_FLAG_FLAGS_H */

// netgame_starting_equipment_flag_flags: DB-verified via types_enum_values _A97CE003292EA707A93C4DE87774387F
