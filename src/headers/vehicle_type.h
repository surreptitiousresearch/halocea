/* DB-verified 2026-07-20 (BLOCKED-retry): values match compiled enum types_enum_values
 * _9579021B0EF87E584E9713AE3E8182DC exactly, including value 6 = turret (DB _vehicle_turret — the prior
 * GUESS label is now confirmed). DB uses the _vehicle_* prefix; the _vehicle_type_* names are kept. */
#ifndef VEHICLE_TYPE_H
#define VEHICLE_TYPE_H

/* vehicle_type — the vehicle tag's physics-model selector (_vehicle_definition.type).
 *
 * Values 0..5 are ground truth from the binary's own DB-named physics drivers dispatched on this field
 * in vehicle_update @0x8375FAB8: 0 -> update_human_tank_physics, 1 -> update_human_jeep_physics,
 * 2 -> update_human_boat_physics, 3 -> update_human_plane_physics, 4 -> update_alien_scout_physics,
 * 5 -> update_alien_fighter_physics.
 * Value 6 = turret is DB-confirmed (DB _vehicle_turret); it takes only the generic physics_update
 * path (no self-propelled physics). DB also carries a lumped mask enumerator _vehicle_mask_flying=0x28
 * (a flag, not a type value) — deliberately not mirrored here. */
enum vehicle_type {
	_vehicle_type_human_tank = 0,
	_vehicle_type_human_jeep = 1,
	_vehicle_type_human_boat = 2,
	_vehicle_type_human_plane = 3,
	_vehicle_type_alien_scout = 4,
	_vehicle_type_alien_fighter = 5,
	_vehicle_type_turret = 6,        /* DB _vehicle_turret */
	NUMBER_OF_VEHICLE_TYPES = 7,
};

#endif /* VEHICLE_TYPE_H */
