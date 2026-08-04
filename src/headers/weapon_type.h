/* DB-verified 2026-07-20 (BLOCKED-retry): values match compiled enum types_enum_values
 * _576107C8C5057FEC7D3D5138DEACB5ED exactly. */
#ifndef WEAPON_TYPE_H
#define WEAPON_TYPE_H

/* weapon_type — the weapon tag's special-case behavior selector (_weapon_definition.weapon_type,
 * +0x1DA). Distinct from the weapon export-function / animation enums. Drives the first-person
 * per-shell reload chain (shotgun), needler node handling and plasma-pistol overheat special cases.
 * DB-AUTHORITATIVE. */
enum weapon_type {
	_weapon_type_undefined = 0,
	_weapon_type_shotgun = 1,
	_weapon_type_needler = 2,
	_weapon_type_plasma_pistol = 3,
	_weapon_type_plasma_rifle = 4,
	NUMBER_OF_WEAPON_TYPES = 5,
};

#endif /* WEAPON_TYPE_H */
