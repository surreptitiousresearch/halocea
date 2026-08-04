#ifndef AI_ATOM_VEHICLE_MODIFIER_H
#define AI_ATOM_VEHICLE_MODIFIER_H

/* ai_atom_vehicle_modifier — seat-desire selector of the _ai_atom_vehicle command atom, also the
 * seat_desire_type argument threaded through actor_action_try_to_enter_vehicle /
 * vehicle_scripting_find_available_seats. DB-verified via types_enum_values
 * $1E32860A4DF8C1EF38F50916084635DF. */
enum ai_atom_vehicle_modifier {
	_ai_atom_vehicle_modifier_any_non_driver = 0,
	_ai_atom_vehicle_modifier_gunner = 1,
	_ai_atom_vehicle_modifier_passenger = 2,
	_ai_atom_vehicle_modifier_driver = 3,
	_ai_atom_vehicle_modifier_any_seat = 4,
	NUMBER_OF_AI_ATOM_VEHICLE_MODIFIERS = 5,
};

#endif /* AI_ATOM_VEHICLE_MODIFIER_H */
