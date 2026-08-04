#ifndef MACHINE_DATUM_FLAGS_H
#define MACHINE_DATUM_FLAGS_H

/* machine_datum_flags — bit indices for machine_datum.machine.flags (device->machine.flags).
 * Source: reference enum _C19FA32B4F612C7790B123E74273FD3F (headers_ref). Values are BIT INDICES — use as (1u << _bit). */
enum machine_datum_flags {
	_machine_does_not_operate_automatically_bit = 0x0,
	_machine_one_sided_bit = 0x1,
	_machine_never_appears_locked_bit = 0x2,
	_machine_opened_by_melee_attack_bit = 0x3,
	NUMBER_OF_MACHINE_DATUM_FLAGS = 0x4,
};

#endif /* MACHINE_DATUM_FLAGS_H */

// machine_datum_flags: DB-verified via types_enum_values _C19FA32B4F612C7790B123E74273FD3F (anonymous compiled enum, names sans leading _)
