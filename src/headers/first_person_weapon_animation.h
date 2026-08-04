#ifndef FIRST_PERSON_WEAPON_ANIMATION_H
#define FIRST_PERSON_WEAPON_ANIMATION_H

/* first_person_weapon_animation — index into the animation_graph's first-person weapon animation block
 * (animation_graph_first_person_weapon_animations.animations).
 * Source: binary label table first_person_weapon_animation_list_entries @0x84185E88 (28 entries of
 * {char *label, 0}); labels read directly from .data, entry order is the enum value. Ground truth. */
/* layout bound to DB enum: types_enum_values _7B5A8DAD25CEEA083D35CBE7F487D53E */
enum first_person_weapon_animation {
	_first_person_weapon_animation_idle = 0x0,
	_first_person_weapon_animation_posing = 0x1,
	_first_person_weapon_animation_primary_fire = 0x2,
	_first_person_weapon_animation_moving = 0x3,
	_first_person_weapon_animation_overlays = 0x4,
	_first_person_weapon_animation_light_on = 0x5,
	_first_person_weapon_animation_light_off = 0x6,
	_first_person_weapon_animation_reload_while_empty = 0x7,
	_first_person_weapon_animation_reload_while_full = 0x8,
	_first_person_weapon_animation_overheated = 0x9,
	_first_person_weapon_animation_ready = 0xA,
	_first_person_weapon_animation_put_away = 0xB,
	_first_person_weapon_animation_overcharged = 0xC,
	_first_person_weapon_animation_melee = 0xD,
	_first_person_weapon_animation_secondary_fire = 0xE,
	_first_person_weapon_animation_overcharged_jitter = 0xF,
	_first_person_weapon_animation_throw_grenade = 0x10,
	_first_person_weapon_animation_ammunition = 0x11,
	_first_person_weapon_animation_primary_misfire = 0x12,
	_first_person_weapon_animation_secondary_misfire = 0x13,
	_first_person_weapon_animation_throw_grenade_overheated = 0x14,
	_first_person_weapon_animation_overheating = 0x15,
	_first_person_weapon_animation_overheating_again = 0x16,
	_first_person_weapon_animation_shotgun_enter = 0x17,
	_first_person_weapon_animation_shotgun_exit_empty = 0x18,
	_first_person_weapon_animation_shotgun_exit_full = 0x19,
	_first_person_weapon_animation_overheated_exit = 0x1A,
	_first_person_weapon_animation_overheated_supercharge_enter = 0x1B,
	NUMBER_OF_FIRST_PERSON_WEAPON_ANIMATIONS = 0x1C,
};

#endif /* FIRST_PERSON_WEAPON_ANIMATION_H */
