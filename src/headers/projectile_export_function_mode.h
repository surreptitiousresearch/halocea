/* DB-verified 2026-07-20 (BLOCKED-retry): values match compiled enum types_enum_values
 * _F25E8FAD12F8FED645CA133D6726DCA7 exactly (DB names use the _projectile_function_* prefix; the
 * _projectile_export_* identifiers below are the consumer-facing names and are kept). */
#ifndef PROJECTILE_EXPORT_FUNCTION_MODE_H
#define PROJECTILE_EXPORT_FUNCTION_MODE_H

/* projectile_export_function_mode — the projectile tag's per-slot export-function source selector
 * (_projectile_definition.function_modes[4], the "A in".."D in" tag fields).
 *
 * GUESS: enum recovered by reconciliation, no ground-truth source in this binary (tag-editor option
 * strings stripped; no enum in headers_ref/DB) — rationale: names are the published Halo CE
 * tag-definition option list, and projectile_export_function_values @0x83759D10 matches it exactly:
 * 1 = odometer / detonation range ("range remaining"), 2 = detonation_timer ("time remaining"),
 * 3 = 1.0 while projectile flags bit 1 is set ("tracer"). */
enum projectile_export_function_mode {
	_projectile_export_none = 0,
	_projectile_export_range_remaining = 1,
	_projectile_export_time_remaining = 2,
	_projectile_export_tracer = 3,
	NUMBER_OF_PROJECTILE_EXPORT_FUNCTION_MODES = 4,
};

#endif /* PROJECTILE_EXPORT_FUNCTION_MODE_H */
