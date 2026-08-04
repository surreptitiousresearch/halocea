#ifndef SHOTGUN_RELOAD_TYPE_H
#define SHOTGUN_RELOAD_TYPE_H

/* Shotgun shell-by-shell reload phase (first_person_weapon shotgun reload overlays). */
enum shotgun_reload_type {
	_shotgun_reload_type_first_round = 0,
	_shotgun_reload_type_last_round = 1,
	_shotgun_reload_type_first_and_last_round = 2,
	NUMBER_OF_SHOTGUN_RELOAD_TYPES = 3,
};

#endif /* SHOTGUN_RELOAD_TYPE_H */

// shotgun_reload_type: DB-verified via types_enum_values $35BDCB829B8D9FBB12584EA047C3D658
