#ifndef WEAPON_FLAGS_H
#define WEAPON_FLAGS_H

/* weapon_flags — runtime flag bit indices for weapon_datum.weapon.flags.
 * DB-verified: the IDA DB names only the oddball bit in anonymous enum
 * _FF589A6A8D38253F78FD4E22AB5255BE (value is a bit index, so 0x40 == 1 << 6). */
enum weapon_flags
{
	_weapon_oddball_flag_has_been_touched_bit = 6,
};

#endif /* WEAPON_FLAGS_H */
