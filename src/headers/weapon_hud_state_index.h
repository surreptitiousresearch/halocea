#pragma once

/* weapon_hud_state_index — the weapon-HUD state slot: the index into render_weapon_hud's parallel
 * per-state arrays (state_flags[8], overlay_flags[8], number_values[8], number_fractions[8]) and into
 * weapon_hud_state.last_weapon_flash_time[8].
 *
 * Confirmed member-by-member from render_weapon_hud @0x836AEA00, which fills the whole domain from
 * named weapon_interface_state fields: number_values[0]=magazines[0].rounds_remaining (total ammo),
 * [1]=magazines[0].rounds_loaded, [2]=heat*255, [3]=(1-age)*100, [4]=magazines[1].rounds_remaining
 * (secondary total), [5]=magazines[1].rounds_loaded (secondary loaded), and number_fractions[6]/[7] =
 * the horizontal range to the locked target / the target-minus-camera vertical delta (distance and
 * elevation to target). Source: types_enum_values $FCC09C7C269C3840BE1EF860801CBDD2 (DB-verified).
 *
 * NAMING: the members' own prefix (_weapon_state_) and this family's count member (WEAPON_HUD_STATES)
 * both collide with existing STRUCT tags that are in scope in the very TU that consumes this enum —
 * src/headers/weapon_state.h (networked weapon snapshot) and src/headers/weapon_hud_state.h
 * (per-local-player flash timing). Struct and enum tags share one namespace in C, so the tag is spelled
 * weapon_hud_state_index here; the member names are DB-verbatim and untouched. Same convention as
 * src/headers/weapon_datum_state.h. */
enum weapon_hud_state_index {
	_weapon_state_total_ammo = 0,
	_weapon_state_loaded_ammo = 1,
	_weapon_state_heat = 2,
	_weapon_state_age = 3,
	_weapon_state_secondary_total_ammo = 4,
	_weapon_state_secondary_loaded_ammo = 5,
	_weapon_state_distance_to_target = 6,
	_weapon_state_elevation_to_target = 7,
	NUMBER_OF_WEAPON_HUD_STATES = 8,
};

// weapon_hud_state_index: DB-verified via types_enum_values $FCC09C7C269C3840BE1EF860801CBDD2
