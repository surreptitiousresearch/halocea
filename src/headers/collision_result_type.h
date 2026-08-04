#ifndef COLLISION_RESULT_TYPE_H
#define COLLISION_RESULT_TYPE_H

/* collision_result_type — what a collision test hit (collision_result.type).
 * Source: reference enum _C2F628667FF40A3B5E6B3AB3BE52BA27 (headers_ref); the IDA DB keeps only the
 * anonymized member-less enum shell. */
enum collision_result_type {
	collision_result_media = 0,
	collision_result_mesh = 1,
	collision_result_structure = 2,
	collision_result_object = 3,
	number_of_collision_result_types = 4
} ;

#endif /* COLLISION_RESULT_TYPE_H */

// collision_result_type: DB-verified via types_enum_values _C2F628667FF40A3B5E6B3AB3BE52BA27 (anonymous compiled enum, names sans leading _)
