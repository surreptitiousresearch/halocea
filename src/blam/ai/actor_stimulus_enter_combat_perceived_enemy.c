/* actor_stimulus_enter_combat_perceived_enemy @0x837D4238 — stimulus that pushes an actor into combat
 * against a perceived (heard/seen) enemy prop: raises a combat-enemy transition stimulus carrying the
 * actor-to-prop direction and the prop index (look timer 150, guard timer 90).
 *
 * DEVIATION: the decompiler output is a verbatim compiler-inlined copy of actor_stimulus_combat
 * @0x837D3C90 (zero call xrefs in the binary — it is inlined at every caller) constant-folded for
 * guard_point=NULL, with the callee's `if (transition_vector)` parameter test surviving on the
 * never-null &prop->actor_to_prop — emitted as the call the source had. The NULL guard_point makes
 * guard_point_surface_index/guard_distance dead; -1/0.0f are the assumed folded-away spellings. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/data_array.h"
#include "headers/real_vector3d.h"
#include "headers/actor_combat_transition_type.h"
#include "headers/blam_data_globals.h"

extern void actor_stimulus_combat(int actor_index, int16_t transition_type, const real_point3d *guard_point, int guard_point_surface_index, float guard_distance, int guard_timer, const real_vector3d *transition_vector, int prop_index, int prop_look_timer, uint8_t prop_look_while_moving);

/* DEVIATION (2026-08-12, #134): prop_index is `int`, a full datum handle, NOT the `uint16_t` this was
 * declared with. `clrlwi r7, r4, 16` @0x837D4254 is only the prop_data subscript (DATA_ARRAY_ELEMENT's
 * own truncation, data_array.h); r4 is never rewritten, and `stw r4, 0x340(r11)` @0x837D42D0 persists
 * the WHOLE word into the actor datum. The inlined callee already declares that slot `int prop_index`
 * (see the extern below) — the narrow parameter was stripping the identifier salt off a handle on its
 * way into 32 bits of persistent game state. */
void actor_stimulus_enter_combat_perceived_enemy(int actor_index, int prop_index)
{
    prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);

    actor_stimulus_combat(actor_index, _actor_stimulus_combat_enemy, 0, -1, 0.0f, 90,
                          &prop->actor_to_prop, prop_index, 150, 0);
}
