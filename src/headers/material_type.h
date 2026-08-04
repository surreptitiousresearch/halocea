#ifndef MATERIAL_TYPE_H
#define MATERIAL_TYPE_H

/* Global surface/damage material types (globals.globals materials block order;
 * mass_point ground/water material, damage material response, effects). */
enum material_type {
	_material_dirt = 0,
	_material_sand = 1,
	_material_stone = 2,
	_material_snow = 3,
	_material_wood = 4,
	_material_hollow_metal = 5,
	_material_thin_metal = 6,
	_material_thick_metal = 7,
	_material_rubber = 8,
	_material_glass = 9,
	_material_force_field = 10,
	_material_grunt = 11,
	_material_hunter_armor = 12,
	_material_hunter_skin = 13,
	_material_elite = 14,
	_material_jackal = 15,
	_material_jackal_energy_shield = 16,
	_material_engineer = 17,
	_material_engineer_force_field = 18,
	_material_flood_combat_form = 19,
	_material_flood_carrier_form = 20,
	_material_cyborg = 21,
	_material_cyborg_energy_shield = 22,
	_material_armored_human = 23,
	_material_human = 24,
	_material_sentinel = 25,
	_material_monitor = 26,
	_material_plastic = 27,
	_material_water = 28,
	_material_leaves = 29,
	_material_elite_energy_shield = 30,
	_material_ice = 31,
	_material_hunter_shield = 32,
	NUMBER_OF_MATERIAL_TYPES = 33,
	MAXIMUM_NUMBER_OF_MATERIAL_TYPES = 40,
};

#endif /* MATERIAL_TYPE_H */

// material_type: DB-verified via types_enum_values $39FD3E89ED057A90C4E82A7D0CE5EE6C
