#ifndef ACTOR_VECTOR_AVOIDANCE_RESULT_H
#define ACTOR_VECTOR_AVOIDANCE_RESULT_H

/* Result code of an actor avoidance-ray test (actor_move_test_avoidance_vector):
 * clear, blocked by an obstacle object (pill), or blocked by structure BSP. */
enum actor_vector_avoidance_result {
	_actor_vector_avoidance_clear = 0,
	_actor_vector_avoidance_obstructed_object = 1,
	_actor_vector_avoidance_obstructed_structure = 2,
	NUMBER_OF_ACTOR_VEHICLE_AVOIDANCE_RESULTS = 3,   /* count name verbatim from DB ("VEHICLE") */
};

#endif /* ACTOR_VECTOR_AVOIDANCE_RESULT_H */

// actor_vector_avoidance_result: DB-verified via types_enum_values $40E023C9CEADC91186A3DDDE03DAFD64
