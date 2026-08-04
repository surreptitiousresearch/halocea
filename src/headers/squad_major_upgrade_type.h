#ifndef SQUAD_MAJOR_UPGRADE_TYPE_H
#define SQUAD_MAJOR_UPGRADE_TYPE_H

/* squad_major_upgrade — scenario squad "major upgrade" policy (how many members spawn as the
 * major variant): normal/few/many use a per-difficulty random chance, none/all force the choice. */
enum squad_major_upgrade_type {
	_squad_major_upgrade_normal = 0,
	_squad_major_upgrade_few = 1,
	_squad_major_upgrade_many = 2,
	SQUAD_RANDOM_MAJOR_UPGRADE_MAX_TYPE = 2,
	_squad_major_upgrade_none = 3,
	_squad_major_upgrade_all = 4,
	NUMBER_OF_SQUAD_MAJOR_UPGRADE_TYPES = 5,
};

#endif /* SQUAD_MAJOR_UPGRADE_TYPE_H */

// squad_major_upgrade_type: DB-verified via types_enum_values $73A0C624AE892E0205526EE7533EBC40
