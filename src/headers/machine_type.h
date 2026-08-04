#ifndef MACHINE_TYPE_H
#define MACHINE_TYPE_H

/* machine_type — _machine_definition.type discriminator ('mach' tag).
 * DB-verified: types_enum_values _D8FDDF69FF972FEFFFD43E162D28AA17 carries these named
 * values verbatim. */
enum machine_type {
	_machine_door = 0,
	_machine_platform = 1,
	_machine_gear = 2,
	NUMBER_OF_MACHINE_TYPES = 3,
};

#endif /* MACHINE_TYPE_H */

// machine_type: DB-verified via types_enum_values _D8FDDF69FF972FEFFFD43E162D28AA17
