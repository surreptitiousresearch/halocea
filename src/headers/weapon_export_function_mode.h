/* DB-verified 2026-07-20 (BLOCKED-retry): values match compiled enum types_enum_values
 * _36BECCA572AD6AD415EBCC716E4A0D82 exactly (DB names use the _weapon_function_* prefix; the
 * _weapon_export_* identifiers below are the consumer-facing names and are kept). */
#ifndef WEAPON_EXPORT_FUNCTION_MODE_H
#define WEAPON_EXPORT_FUNCTION_MODE_H

/* weapon_export_function_mode — the weapon tag's per-slot export-function source selector
 * (_weapon_definition.function_modes[4], the "A in".."D in" tag fields).
 *
 * GUESS: enum recovered by reconciliation, no ground-truth source in this binary (tag-editor option
 * strings stripped; no enum in headers_ref/DB) — rationale: names are the published Halo CE
 * tag-definition option list, and weapon_export_function_values @0x836DAD30 matches it exactly:
 * 1 heat, 2/3 magazine rounds fraction (primary/secondary ammunition), 4/5 trigger rate_of_fire
 * (primary/secondary rate of fire), 6 constant 1.0 ("ready"), 7/8 trigger ejection_port_position,
 * 9 overheat fraction while the overheated flag is set, 10/11 trigger charged fraction,
 * 12 max illumination across triggers/heat ("illumination"), 13 age, 14 integrated_light_power,
 * 15/16 rate_of_fire gated by time-since-last-shot (primary/secondary firing), 17/18 rate_of_fire
 * gated by loaded rounds / overheat / reloading (primary/secondary firing on). */
enum weapon_export_function_mode {
	_weapon_export_none = 0,
	_weapon_export_heat = 1,
	_weapon_export_primary_ammunition = 2,
	_weapon_export_secondary_ammunition = 3,
	_weapon_export_primary_rate_of_fire = 4,
	_weapon_export_secondary_rate_of_fire = 5,
	_weapon_export_ready = 6,
	_weapon_export_primary_ejection_port = 7,
	_weapon_export_secondary_ejection_port = 8,
	_weapon_export_overheated = 9,
	_weapon_export_primary_charged = 10,
	_weapon_export_secondary_charged = 11,
	_weapon_export_illumination = 12,
	_weapon_export_age = 13,
	_weapon_export_integrated_light = 14,
	_weapon_export_primary_firing = 15,
	_weapon_export_secondary_firing = 16,
	_weapon_export_primary_firing_on = 17,
	_weapon_export_secondary_firing_on = 18,
	NUMBER_OF_WEAPON_EXPORT_FUNCTION_MODES = 19,
};

#endif /* WEAPON_EXPORT_FUNCTION_MODE_H */
