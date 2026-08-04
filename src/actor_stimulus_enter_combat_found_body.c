/* actor_stimulus_enter_combat_found_body @0x837D415C — stimulus for an actor discovering an ally's body.
 * Stamps the discovery time, finds a pathfinding location to the body's prop, and raises a combat-body
 * transition stimulus guarding at the body's pathfinding point (distance 1.5, timers 90 ticks,
 * look-while-moving).
 *
 * DEVIATION: everything after the pathfinding lookup in the decompiler output is a verbatim
 * compiler-inlined copy of actor_stimulus_combat @0x837D3C90 (zero call xrefs in the binary — it is
 * inlined at every caller), with guard_point = &prop->pathfinding_point and transition_vector = NULL
 * folded — emitted as the call the source had. The decompiler's odd "prop == (prop_datum *)-240"
 * predicate (disasm 0x837D41A0: addi r10,r31,0xF0 ; cmplwi r10,0) was the callee's own
 * `if (guard_point)` parameter test applied to the never-null &prop->pathfinding_point. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/actor_combat_transition_type.h"
#include "headers/blam_data_globals.h"

extern int game_time_get(void);
extern void actor_perception_find_prop_pathfinding_location(uint16_t actor_index, uint16_t prop_index);
extern void actor_stimulus_combat(int actor_index, int16_t transition_type, const real_point3d *guard_point, int guard_point_surface_index, float guard_distance, int guard_timer, const real_vector3d *transition_vector, int prop_index, int prop_look_timer, uint8_t prop_look_while_moving);

void actor_stimulus_enter_combat_found_body(uint16_t actor_index, uint16_t prop_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);

    actor->emotions.corpse_ignore_time = game_time_get();
    actor_perception_find_prop_pathfinding_location(actor_index, prop_index);

    actor_stimulus_combat(actor_index, _actor_stimulus_combat_body, &prop->pathfinding_point,
                          prop->pathfinding_surface_index, 1.5f, 90, 0, prop_index, 90, 1);
}
