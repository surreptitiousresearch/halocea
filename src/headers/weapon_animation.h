#ifndef WEAPON_ANIMATION_H
#define WEAPON_ANIMATION_H

/* weapon_animation — index into a weapon animation graph's weapon_animations block
 * (animation_graph_weapon_animations.animations; used by weapon_set_state).
 * Source: binary label table weapon_animation_list_entries @0x84185F70 (11 entries of
 * {char *label, 0}); labels read directly from .data, entry order is the enum value. Ground truth. */
/* layout bound to DB enum: types_enum_values _4DC3AAE2F12818BC7C5F102651731F44 */
enum weapon_animation {
	_weapon_animation_idle = 0x0,
	_weapon_animation_ready = 0x1,
	_weapon_animation_put_away = 0x2,
	_weapon_animation_primary_reload = 0x3,
	_weapon_animation_secondary_reload = 0x4,
	_weapon_animation_primary_chamber = 0x5,
	_weapon_animation_secondary_chamber = 0x6,
	_weapon_animation_primary_charged = 0x7,
	_weapon_animation_secondary_charged = 0x8,
	_weapon_animation_primary_recoil = 0x9,
	_weapon_animation_secondary_recoil = 0xA,
	NUMBER_OF_WEAPON_ANIMATIONS = 0xB,
};

#endif /* WEAPON_ANIMATION_H */
