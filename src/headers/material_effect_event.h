#ifndef MATERIAL_EFFECT_EVENT_H
#define MATERIAL_EFFECT_EVENT_H

/* material_effect_event — the event index selecting which sub-effect of a material_effects tag to
 * spawn (biped_make_footstep / material_effect_new_from_point event_index argument).
 * Source: reference enum _F379257C29B3B72E6B5E2DCB00370839 (types_enum_values; anonymous compiled
 * enum, count NUMBER_OF_MATERIAL_EFFECT_TYPES = 13). */
enum material_effect_event {
	_material_effect_biped_walk              = 0,
	_material_effect_biped_run               = 1,
	_material_effect_biped_sliding           = 2,
	_material_effect_biped_shuffle           = 3,
	_material_effect_biped_jump              = 4,
	_material_effect_biped_jump_land         = 5,
	_material_effect_biped_unused1           = 6,
	_material_effect_biped_unused2           = 7,
	_material_effect_impact                  = 8,
	_material_effect_vehicle_slip_nonmetallic = 9,
	_material_effect_vehicle_slip_metallic   = 10,
	_material_effect_vehicle_unused1         = 11,
	_material_effect_vehicle_unused2         = 12,
	NUMBER_OF_MATERIAL_EFFECT_TYPES          = 13,
};

#endif /* MATERIAL_EFFECT_EVENT_H */
