#pragma once

/* collision_feature — the kind of collision feature gathered by collision_get_features_in_sphere,
 * and the index into the three parallel slots of collision_feature_list: count[3] alongside
 * spheres[256] / cylinders[256] / prisms[256]. The pairing is explicit at every producer site —
 * collision_features_from_point.c writes spheres[count[0]++] and cylinders[count[1]++],
 * collision_features_from_line.c writes cylinders[count[1]++] and prisms[count[2]++], and
 * collision_features_from_polygon.c writes prisms[count[2]++].
 * Source: types_enum_values $BF4A7470B716A64ED9BBEA0C99BAFC4D (DB-verified). */
enum collision_feature {
	_collision_feature_sphere = 0,
	_collision_feature_cylinder = 1,
	_collision_feature_prism = 2,
	NUMBER_OF_COLLISION_FEATURE_TYPES = 3,
};

// collision_feature: DB-verified via types_enum_values $BF4A7470B716A64ED9BBEA0C99BAFC4D (anonymous compiled enum, names verbatim)
