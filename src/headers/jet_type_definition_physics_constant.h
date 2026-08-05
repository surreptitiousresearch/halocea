#pragma once
/* jet_type_definition_physics_constant — index into a particle_system_type's physics_constants
 * tag_block (each element is one real) when that type's initial_particle_creation_physics is
 * _particle_system_type_create_jet. "rotates up" is stored as a real and tested against 0.0.
 * DB anonymous enum $47A9551BAC7CA9E1DCA8D055E44125ED (types_enum_values). */

enum jet_type_definition_physics_constant {
    _jet_type_definition_physics_constant_velocity = 0,
    _jet_type_definition_physics_constant_spread_fraction = 1,
    _jet_type_definition_physics_constant_rotates_up = 2,
    NUMBER_OF_JET_TYPE_PHYSICS_CONSTANTS = 3,
};
