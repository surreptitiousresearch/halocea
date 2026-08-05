#pragma once
/* explosion_type_definition_physics_constant — index into a particle_system_type's physics_constants
 * tag_block (each element is one real) when that type's initial_particle_creation_physics is
 * _particle_system_type_create_explosion.
 * DB anonymous enum $8462BD632A1D557F4052146EBE8C1CB1 (types_enum_values). */

enum explosion_type_definition_physics_constant {
    _explosion_type_definition_physics_constant_xy_spread = 0,
    _explosion_type_definition_physics_constant_z_spread = 1,
    _explosion_type_definition_physics_constant_intensity = 2,
    NUMBER_OF_EXPLOSION_TYPE_PHYSICS_CONSTANTS = 3,
};
