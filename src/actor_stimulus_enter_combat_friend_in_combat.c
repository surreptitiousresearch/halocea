/* actor_stimulus_enter_combat_friend_in_combat @0x837D42F8 — stimulus applied to an actor when a friendly
 * prop (a known threat/target shared by the squad) reports being in combat. If the actor isn't already in
 * a higher combat state it transitions into combat, latching the prop's last position and a set of combat
 * timers. It then raises the actor's combat-awareness level toward the referenced actor's level, refreshing
 * the awareness timer.
 *
 * DEVIATION: the first block of the decompiler output is a verbatim compiler-inlined copy of
 * actor_stimulus_combat @0x837D3C90 (zero call xrefs in the binary — it is inlined at every caller)
 * constant-folded for guard_point=NULL — emitted as the call the source had. The `&prop->actor_to_prop
 * == 0` always-false artifact (v5 == -224) was the callee's own `if (transition_vector)` parameter
 * test. The NULL guard_point makes guard_point_surface_index/guard_distance dead; -1/0.0f are the
 * assumed folded-away spellings. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/prop_datum.h"
#include "headers/actor_combat_transition_type.h"
#include "headers/blam_data_globals.h"

extern void actor_stimulus_combat(int actor_index, int16_t transition_type, const real_point3d *guard_point, int guard_point_surface_index, float guard_distance, int guard_timer, const real_vector3d *transition_vector, int prop_index, int prop_look_timer, uint8_t prop_look_while_moving);

void actor_stimulus_enter_combat_friend_in_combat(uint16_t actor_index, uint16_t prop_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);

    actor_stimulus_combat(actor_index, _actor_stimulus_combat_friend, 0, -1, 0.0f, 90,
                          &prop->actor_to_prop, prop_index, 150, 0);

    if (prop->actor_index != -1)
    {
        actor_datum *referenced = DATA_ARRAY_ELEMENT(actor_data, actor_datum, prop->actor_index);
        __int16 referenced_level = referenced->stimuli.suspicion_combat_status;
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
