/* actor_stimulus_combat @0x837D3C90 — request a transition of an actor into the combat state, recording
 * the stimulus payload into the actor's pending-transition block. Ignored when the actor is already at
 * combat awareness (state word +106 >= 3) or a stronger transition is pending (+786). Raw actor offsets
 * (disasm-confirmed): transition type word 786, has-guard-point byte 788, guard point 792/796/800, guard
 * surface 804, guard distance 808, has-transition-vector byte 812, transition vector 816..824, guard timer
 * 828, prop index 832, prop look timer 836, prop-look-while-moving byte 840.
 *
 * DEVIATION (bug classes 1+2): guard_distance (f1) burns the r7 slot, and the decompiler both invented 21
 * phantom parameters and shifted the tail arguments (rendering the transition-vector copy as prop_index[]
 * reads and storing the vector POINTER into a field). Register-level disasm gives the mapping used here,
 * which matches the DB's 10-arg prototype exactly. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/actor_mode.h"
#include "headers/blam_data_globals.h"


void actor_stimulus_combat(int actor_index, int16_t transition_type, const real_point3d *guard_point, int guard_point_surface_index, float guard_distance, int guard_timer, const real_vector3d *transition_vector, int prop_index, int prop_look_timer, uint8_t prop_look_while_moving)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( actor->state.mode >= _actor_mode_combat )
        return;
    if ( transition_type < actor->stimuli.combat_transition )
        return;
    actor->stimuli.combat_transition = transition_type;

    if ( guard_point )
    {
        actor->stimuli.combat_transition_guard_at_point = 1;
        actor->stimuli.combat_transition_guard_point.n[0] = guard_point->n[0];
        actor->stimuli.combat_transition_guard_point.n[1] = guard_point->n[1];
        actor->stimuli.combat_transition_guard_point.n[2] = guard_point->n[2];
        actor->stimuli.combat_transition_guard_point_distance = guard_distance;
        actor->stimuli.combat_transition_guard_point_surface_index = guard_point_surface_index;
    }
    else
    {
        actor->stimuli.combat_transition_guard_at_point = 0;
    }

    if ( transition_vector )
    {
        actor->stimuli.combat_transition_has_vector = 1;
        actor->stimuli.combat_transition_vector.n[0] = transition_vector->n[0];
        actor->stimuli.combat_transition_vector.n[1] = transition_vector->n[1];
        actor->stimuli.combat_transition_vector.n[2] = transition_vector->n[2];
    }
    else
    {
        actor->stimuli.combat_transition_has_vector = 0;
    }

    actor->stimuli.combat_transition_prop_index = prop_index;
    actor->stimuli.combat_transition_guard_timer = guard_timer;
    actor->stimuli.combat_transition_prop_look_timer = prop_look_timer;
    actor->stimuli.combat_transition_prop_look_while_moving = prop_look_while_moving;
}
