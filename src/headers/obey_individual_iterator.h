#pragma once
/* obey_individual_iterator — the per-individual callback that action_obey_individuals_iterate
 * drives over an actor's controllable individuals.
 *
 * For a single-unit actor the callback runs once with the actor's unit and the shared
 * obey_state_data simple/complex controls; for a swarm actor it runs once per live swarm
 * component, receiving that component's overlaid simple control and a null complex control.
 *
 * Arity/widths are taken from action_obey_individuals_iterate's call sites: r3 actor_index,
 * r4 unit_index, r5 command_list_index (sign-extended 16-bit), r6/r7 control pointers,
 * r8 user_data. */

#include <stdint.h>
#include "obey_individual_simple_control.h"
#include "obey_individual_complex_control.h"

typedef void (*obey_individual_iterator)(int actor_index, int unit_index, int16_t command_list_index,
                                         obey_individual_simple_control *simple_control,
                                         obey_individual_complex_control *complex_control, void *user_data);
