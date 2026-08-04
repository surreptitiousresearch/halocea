#ifndef HUD_WEAPON_OVERLAY_FLAGS_H
#define HUD_WEAPON_OVERLAY_FLAGS_H

/* hud_weapon_overlay_flags — hud_weapon_overlay flags.
 * DB-verified: types_enum_values _4D15DD04DC6FDDBF610FE51D08446933 carries these named
 * values verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum hud_weapon_overlay_flags {
	_hud_overlay_flashes_bit = 0,
	NUMBER_OF_HUD_WEAPON_OVERLAY_FLAGS = 1,
	_hud_overlay_runtime_invalid_bit = 1,
	NUMBER_OF_HUD_WEAPON_OVERLAY_RUNTIME_FLAGS = 2,
};

#endif /* HUD_WEAPON_OVERLAY_FLAGS_H */

// hud_weapon_overlay_flags: DB-verified via types_enum_values _4D15DD04DC6FDDBF610FE51D08446933
