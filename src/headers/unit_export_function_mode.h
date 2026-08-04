/* DB-verified 2026-07-20 (BLOCKED-retry): values match compiled enum types_enum_values
 * _8A7DAE123CB387FA53F24459A62F58FC exactly, including value 7 = shield_sapping (DB
 * _unit_function_shield_sapping — the prior GUESS label is now confirmed). DB names use the
 * _unit_function_* prefix; the _unit_export_* identifiers below are the consumer-facing names. */
#ifndef UNIT_EXPORT_FUNCTION_MODE_H
#define UNIT_EXPORT_FUNCTION_MODE_H

/* unit_export_function_mode — the unit tag's per-slot export-function source selector
 * (_unit_definition.function_modes[4], the "A in".."D in" tag fields).
 *
 * GUESS: enum recovered by reconciliation, no ground-truth source in this binary (tag-editor option
 * strings stripped; no enum in headers_ref/DB) — rationale: names 0..6 are the published Halo CE
 * tag-definition option list, and unit_export_function_values @0x836C6D10 matches it exactly:
 * 1/2 read seat_power[0]/[1] (driver/gunner seat power), 3 aiming_change/255, 4 mouth_aperture,
 * 5 integrated_light_power, 6 is 0 when dead or the cannot-blink flag is set ("can blink").
 * Value 7 is an HCEX/late addition beyond the stock list: it computes a normalized animation frame
 * position, falling back to 1 - shield_sap_timeout/90 — named after that shield-sapping fallback;
 * this member's label is the least certain. */
enum unit_export_function_mode {
	_unit_export_none = 0,
	_unit_export_driver_seat_power = 1,
	_unit_export_gunner_seat_power = 2,
	_unit_export_aiming_change = 3,
	_unit_export_mouth_aperture = 4,
	_unit_export_integrated_light_power = 5,
	_unit_export_can_blink = 6,
	_unit_export_shield_sapping = 7, /* GUESS label; frame position / shield-sap fallback */
	NUMBER_OF_UNIT_EXPORT_FUNCTION_MODES = 8,
};

#endif /* UNIT_EXPORT_FUNCTION_MODE_H */
