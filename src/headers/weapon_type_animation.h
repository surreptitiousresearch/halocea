/* DB-verified 2026-07-20 (BLOCKED-retry): the numeric order matches compiled enum types_enum_values
 * _E05CE2F7E66CD08CAE5E3AA72768AE33 (DB uses the _unit_weapon_type_animation_* prefix with primary/
 * secondary naming: 0/1 primary/secondary_reload, 2/3 primary/secondary_chamber, 4/5 primary/secondary
 * _recoil (local "fire_1/2"), 6/7 primary/secondary_charged, 8 melee, 9 overheat). The _weapon_type_
 * animation_* identifiers below are the consumer-facing names and are kept. */
#ifndef WEAPON_TYPE_ANIMATION_H
#define WEAPON_TYPE_ANIMATION_H

/* weapon_type_animation — index into an animation_graph_weapon_type's animations block
 * (the per-weapon-type unit animation table).
 * Source: binary label table weapon_type_animation_list_entries @0x84185B28 (10 entries of
 * {char *label, 0}); labels read directly from .data, entry order is the enum value. Ground truth. */
enum weapon_type_animation {
	_weapon_type_animation_reload_1 = 0,  /* "reload-1" */
	_weapon_type_animation_reload_2 = 1,  /* "reload-2" */
	_weapon_type_animation_chamber_1 = 2, /* "chamber-1" */
	_weapon_type_animation_chamber_2 = 3, /* "chamber-2" */
	_weapon_type_animation_fire_1 = 4,    /* "fire-1" */
	_weapon_type_animation_fire_2 = 5,    /* "fire-2" */
	_weapon_type_animation_charged_1 = 6, /* "charged-1" */
	_weapon_type_animation_charged_2 = 7, /* "charged-2" */
	_weapon_type_animation_melee = 8,     /* "melee" */
	_weapon_type_animation_overheat = 9,  /* "overheat" */
	NUMBER_OF_WEAPON_TYPE_ANIMATIONS = 10,
};

#endif /* WEAPON_TYPE_ANIMATION_H */
