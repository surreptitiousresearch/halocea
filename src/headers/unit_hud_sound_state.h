#ifndef UNIT_HUD_SOUND_STATE_H
#define UNIT_HUD_SOUND_STATE_H

/* unit_hud_sound_state — bit indices for the unit HUD warning-sound condition bitfield built by
 * hud_play_unit_sounds and consumed by hud_play_sound.
 * DB-verified: types_enum_values $4F3E882FB0BAA1307C2B496DA45C66B2 carries these named values
 * verbatim. Values are BIT INDICES — use as (1u << _bit). */
enum unit_hud_sound_state {
	_unit_hud_shield_recharging = 0,
	_unit_hud_shield_damage = 1,
	_unit_hud_shield_low = 2,
	_unit_hud_shield_empty = 3,
	_unit_hud_health_low = 4,
	_unit_hud_health_empty = 5,
	_unit_hud_minor_damage = 6,
	_unit_hud_major_damage = 7,
	NUMBER_OF_UNIT_HUD_STATES = 8,
};

#endif /* UNIT_HUD_SOUND_STATE_H */

// unit_hud_sound_state: DB-verified via types_enum_values $4F3E882FB0BAA1307C2B496DA45C66B2
