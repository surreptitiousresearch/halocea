/* actor_type_swarm_aim_jump @0x837F8FC8 — dispatch to the actor type's swarm "aim jump" handler, if
 * the type defines one. The actor's type index (actor datum +4) selects the actor_type_definition;
 * its swarm_aim_jump function pointer (if set) is tail-called with all of this function's arguments.
 *
 * DEVIATION: the call is a tail call that forwards r3/r4/f1/r5 unchanged; the decompiler rendered it
 * as swarm_aim_jump(jump_magnitude) only. All four args are forwarded (disasm: bctr with no arg setup). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_type_definition.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"

extern actor_type_definition  *actor_type_definitions[16];

void actor_type_swarm_aim_jump(int actor_index, int unit_index, float jump_magnitude,
                               real_vector3d *jump_velocity)
{
    int16_t actor_type = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index)->meta.type;
    void (*swarm_aim_jump)(int actor_index, int unit_index, float jump_magnitude, real_vector3d *jump_velocity) =
        actor_type_definitions[actor_type]->swarm_aim_jump;
    if (swarm_aim_jump)
        swarm_aim_jump(actor_index, unit_index, jump_magnitude, jump_velocity);
}
