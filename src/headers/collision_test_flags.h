#ifndef COLLISION_TEST_FLAGS_H
#define COLLISION_TEST_FLAGS_H

/* collision_test_flags - bit indices for the collision_test.flags word.
 * DB-verified: types_enum_values _7278357605E8DB656D8B016C81954930 carries these named bit indices verbatim.
 * Values are BIT INDICES (single-bit members) - use as (1u << _bit); members named
 * *_flags are DB-provided composite masks (literal mask values, use directly). */
enum collision_test_flags {
	_collision_test_front_facing_surfaces_bit = 0,
	_collision_test_back_facing_surfaces_bit = 1,
	_collision_test_ignore_two_sided_surfaces_bit = 2,
	_collision_test_ignore_invisible_surfaces_bit = 3,
	_collision_test_ignore_breakable_surfaces_bit = 4,
	_collision_test_structure_bit = 5,
	_collision_test_media_bit = 6,
	_collision_test_objects_bit = 7,
	_collision_test_objects_first_type_bit = 8,
	_collision_test_objects_bipeds_bit = 8,
	_collision_test_objects_vehicles_bit = 9,
	_collision_test_objects_weapons_bit = 10,
	_collision_test_objects_equipment_bit = 11,
	_collision_test_objects_garbage_bit = 12,
	_collision_test_objects_projectiles_bit = 13,
	_collision_test_objects_scenery_bit = 14,
	_collision_test_objects_machines_bit = 15,
	_collision_test_objects_controls_bit = 16,
	_collision_test_objects_light_fixtures_bit = 17,
	_collision_test_objects_placeholders_bit = 18,
	_collision_test_objects_last_type_bit = 19,
	_collision_test_try_to_keep_location_valid_bit = 20,
	_collision_test_skip_passthrough_bipeds_bit = 21,
	_collision_test_use_vehicle_physics_bit = 22,
	NUMBER_OF_COLLISION_TEST_FLAGS = 23,
	_collision_test_objects_sight_blocking_flags = 49664,
	_collision_test_objects_all_types_flags = 1048320,
	_collision_test_environment_flags = 97,
	_collision_test_for_projectiles_flags = 1048809,
	_collision_test_for_projectiles_fat_flags = 137,
	_collision_test_for_line_of_sight_flags = 49837,
	_collision_test_for_bipeds_living_flags = 2147232,
	_collision_test_for_bipeds_passthrough_living_flags = 49824,
	_collision_test_for_bipeds_dead_flags = 49312,
	_collision_test_for_vehicles_flags = 49312,
};

#endif /* COLLISION_TEST_FLAGS_H */

// collision_test_flags: DB-verified via types_enum_values _7278357605E8DB656D8B016C81954930
