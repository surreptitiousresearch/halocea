#ifndef WEAPON_LIST_INDEX_H
#define WEAPON_LIST_INDEX_H

/* weapon_list_index — slot order of the game globals weapon_list tag block (globals\globals 'matg',
 * block +0x14C), as compiled into the multiplayer weapon-set remap tables.
 * Source: DB anonymous enum _D2E28AB41F2B267E172CD7FB75855B4D (types_enum_values, verbatim). */
enum weapon_list_index {
	_weapon_list_assault_rifle = 0,
	_weapon_list_flamethrower = 1,
	_weapon_list_gravity_rifle = 2,
	_weapon_list_needler = 3,
	_weapon_list_pistol = 4,
	_weapon_list_plasma_pistol = 5,
	_weapon_list_plasma_rifle = 6,
	_weapon_list_rocket_launcher = 7,
	_weapon_list_shotgun = 8,
	_weapon_list_sniper_rifle = 9,
	_weapon_list_ball = 10,
	_weapon_list_flag = 11,
	_weapon_list_frag_grenade = 12,
	_weapon_list_plasma_grenade = 13,
	_weapon_list_plasma_cannon = 14,
	_weapon_list_mp_needler = 15,
	WEAPON_LIST_NUM_WEAPONS = 16,
};

#endif /* WEAPON_LIST_INDEX_H */
