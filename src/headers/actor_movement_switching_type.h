#ifndef ACTOR_MOVEMENT_SWITCHING_TYPE_H
#define ACTOR_MOVEMENT_SWITCHING_TYPE_H

/* actor_movement_switching_type — actor_variant_definition movement_switching.movement_type.
 * DB-verified: types_enum_values $878E09D3CAB4DA25F4658CA9DC60F96A (verbatim). */
enum actor_movement_switching_type
{
    _actor_movement_switching_always_run = 0,
    _actor_movement_switching_always_crouch = 1,
    _actor_movement_switching_switch_types = 2,
    NUMBER_OF_MOVEMENT_SWITCHING_TYPES = 3
};

#endif /* ACTOR_MOVEMENT_SWITCHING_TYPE_H */
