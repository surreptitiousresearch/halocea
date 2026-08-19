/* actor_stimulus_enter_combat_friend_in_combat @0x837D42E8 — stimulus applied to an actor when a friendly
 * prop (a known threat/target shared by the squad) reports being in combat. If the actor isn't already in
 * a higher combat state it transitions into combat, latching the prop's last position and a set of combat
 * timers. It then raises the actor's combat-awareness level toward the referenced actor's level, refreshing
 * the awareness timer.
 *
 * DEVIATION: the first block of the decompiler output is a verbatim compiler-inlined copy of
 * actor_stimulus_combat @0x837D3C90 (zero call xrefs in the binary — it is inlined at every caller)
 * constant-folded for guard_point=NULL — emitted as the call the source had. The `&prop->actor_to_prop
 * == 0` always-false artifact (the prop pointer compared against -224 = actor_to_prop's own +0xE0)
 * was the callee's own `if (transition_vector)` parameter test. The NULL guard_point makes
 * guard_point_surface_index/guard_distance dead; -1/0.0f are the assumed folded-away spellings. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/actor_combat_transition_type.h"
#include "headers/blam_data_globals.h"

extern void actor_stimulus_combat(int actor_index, int16_t transition_type, const real_point3d *guard_point, int guard_point_surface_index, float guard_distance, int guard_timer, const real_vector3d *transition_vector, int prop_index, int prop_look_timer, uint8_t prop_look_while_moving);

/* DEVIATION (2026-08-12, #134): prop_index is `int`, a full datum handle, NOT the `uint16_t` this was
 * declared with. `clrlwi r9, r4, 16` @0x837D42F4 is only the prop_data subscript (DATA_ARRAY_ELEMENT's
 * own truncation, data_array.h); r4 is never rewritten across the 71-instruction body, and
 * `stw r4, 0x340(r11)` @0x837D4384 persists the WHOLE word into the actor datum. Same field, same
 * mechanism as the perceived_enemy and found_body siblings. */
void actor_stimulus_enter_combat_friend_in_combat(int actor_index, int prop_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);

    actor_stimulus_combat(actor_index, _actor_stimulus_combat_friend, 0, -1, 0.0f, 90,
                          &prop->actor_to_prop, prop_index, 150, 0);

    if (prop->actor_index != -1)
    {
        actor_datum *referenced = DATA_ARRAY_ELEMENT(actor_data, actor_datum, prop->actor_index);
        int16_t referenced_level = referenced->stimuli.suspicion_combat_status;
        if (referenced_level > 0)
        {
            int current_level = actor->stimuli.suspicion_combat_status;
            if (current_level >= referenced_level)
            {
                if (current_level == referenced_level)
                {
                    int timer = actor->stimuli.suspicion_timer;
                    if (timer <= 450)
                        timer = 450;
                    actor->stimuli.suspicion_timer = timer;
                }
            }
            else
            {
                actor->stimuli.suspicion_combat_status = referenced_level;
                actor->stimuli.suspicion_timer = 450;
            }
        }
    }
}
