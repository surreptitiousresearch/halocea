/* DB-verified 2026-07-20 (BLOCKED-retry): values match compiled enum types_enum_values
 * _21D9CCA2768A83EBF37CEF7AEA1557B1 (DB names use the _object_function_* prefix; the _object_export_*
 * identifiers below are the consumer-facing names and are kept). DB collapses regions 10..17 to
 * _object_function_first_region_damage / _object_function_last_region_damage. */
#ifndef OBJECT_EXPORT_FUNCTION_MODE_H
#define OBJECT_EXPORT_FUNCTION_MODE_H

/* object_export_function_mode — the object tag's per-slot export-function source selector
 * (_object_definition.function_modes[4], the "A in"/"B in"/"C in"/"D in" tag fields).
 *
 * GUESS: enum recovered by reconciliation, no ground-truth source in this binary (tag-editor option
 * strings are stripped from release builds; no enum in headers_ref/DB) — rationale: names are the
 * published Halo CE tag-definition option list for this field, and every case's semantics in
 * object_export_function_values @0x836EFBE8 matches it exactly: 1 reads body vitality, 2 shield
 * vitality, 3/4 the recent body/shield damage accumulators, 5 reseeds a random constant, 10..17 index
 * region_damage[source-10] (the eight region damage entries), 18 is 1.0 unless the object is dead
 * ("alive"), 19 derives a [0,1] compass heading from the aiming vector vs scenario north. */
enum object_export_function_mode {
	_object_export_none = 0,
	_object_export_body_vitality = 1,
	_object_export_shield_vitality = 2,
	_object_export_recent_body_damage = 3,
	_object_export_recent_shield_damage = 4,
	_object_export_random_constant = 5,
	_object_export_umbrella_shield_vitality = 6,
	_object_export_shield_stun = 7,
	_object_export_recent_umbrella_shield_vitality = 8,
	_object_export_recent_umbrella_shield_stun = 9,
	_object_export_region_00_damage = 10,
	_object_export_region_01_damage = 11,
	_object_export_region_02_damage = 12,
	_object_export_region_03_damage = 13,
	_object_export_region_04_damage = 14,
	_object_export_region_05_damage = 15,
	_object_export_region_06_damage = 16,
	_object_export_region_07_damage = 17,
	_object_export_alive = 18,
	_object_export_compass = 19,
	NUMBER_OF_OBJECT_EXPORT_FUNCTION_MODES = 20,
};

#endif /* OBJECT_EXPORT_FUNCTION_MODE_H */
