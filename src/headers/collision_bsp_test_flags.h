#ifndef COLLISION_BSP_TEST_FLAGS_H
#define COLLISION_BSP_TEST_FLAGS_H

/* collision_bsp_test_flags — bit indices for test_vector_data.flags (BSP-test flag word).
 * Source: reference enum _BDED26744BF7C1A619C73704AC1EE893 (DB). Values are BIT INDICES — use as (1u << _bit). */
enum collision_bsp_test_flags {
	_collision_bsp_test_front_facing_surfaces_bit = 0x0,
	_collision_bsp_test_back_facing_surfaces_bit = 0x1,
	_collision_bsp_test_ignore_two_sided_surfaces_bit = 0x2,
	_collision_bsp_test_ignore_invisible_surfaces_bit = 0x3,
	_collision_bsp_test_ignore_breakable_surfaces_bit = 0x4,
	NUMBER_OF_COLLISION_BSP_TEST_FLAGS = 0x5,
};

#endif /* COLLISION_BSP_TEST_FLAGS_H */

// collision_bsp_test_flags: DB-verified via types_enum_values _BDED26744BF7C1A619C73704AC1EE893 (anonymous compiled enum, names sans leading _)
