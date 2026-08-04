#ifndef MACHINE_FLAGS_H
#define MACHINE_FLAGS_H

/* machine_flags — machine flags.
 * DB-verified: types_enum_values _3CE7A591B1038A873A065996ED0CC959 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum machine_flags {
	_machine_is_pathfinding_obstacle_bit = 0,
	_machine_is_not_pathfinding_obstacle_when_open_bit = 1,
	_machine_is_elevator_bit = 2,
	NUMBER_OF_MACHINE_FLAGS = 3,
};

#endif /* MACHINE_FLAGS_H */

// machine_flags: DB-verified via types_enum_values _3CE7A591B1038A873A065996ED0CC959
