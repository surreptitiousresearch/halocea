/* action_avoid_perform @0x838212C0 — per-tick "avoid" action update: if the actor has a pending obstacle to
 * avoid (+76 flag), re-evaluate its firing position (forcing evaluation_mode 6, "avoid") via
 * actor_active_select_firing_position and commit the result via actor_change_firing_position. Returns
 * whether the actor still has no assigned firing position (+0x280 word == 0).
 *
 * DEVIATION: the decompiler completely misread the giant (~66KB) local stack frame — needed for the huge
 * firing_position_evaluation_context/path_state scratch buffers — fabricating 48 phantom int parameters and
 * rendering the compiler's stack-overflow probe call (`bl _RtlCheckStack12`, which does not touch r3) as
 * `v48 = RtlCheckStack12(actor_index)`. Disasm (0x83821298-0x83821348) confirms r3 (actor_index) survives
 * the probe call untouched and funcs.prototype confirms the real signature takes only `actor_index`. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/firing_position_evaluation_context.h"
#include "headers/firing_position.h"
#include "headers/path_state.h"
#include "headers/actor_danger_zone_type.h"
#include "headers/firing_point_evaluation_mode.h"
#include "headers/blam_data_globals.h"

extern void *memset(void *dst, int value, unsigned int n);

extern __int16 actor_active_select_firing_position(int actor_index, firing_position_evaluation_context *context,
        firing_position *best_firing_position, int *current_owner, path_state *area_path_state,
        unsigned __int8 *cached_path_available);
extern int16_t actor_change_firing_position(int actor_index, int16_t firing_position_index, firing_position *firing_position, int previous_owner, path_state *cached_path_state, uint8_t cached_path_available);

unsigned __int8 action_avoid_perform(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( actor->meta.timeslice )
    {
        firing_position_evaluation_context evaluation_context;
        firing_position best_firing_position;
        path_state area_path_state;
        int current_owner;
        unsigned __int8 cached_path_available;

        memset(&evaluation_context, 0, sizeof(evaluation_context));
        evaluation_context.evaluation_mode = _firing_point_evaluation_mode_avoid;

        __int16 firing_position_index = actor_active_select_firing_position(actor_index, &evaluation_context,
                &best_firing_position, &current_owner, &area_path_state, &cached_path_available);

        actor_change_firing_position(actor_index, firing_position_index, &best_firing_position, current_owner,
                &area_path_state, cached_path_available);
    }

    return actor->danger_zone.danger_type == actor_danger_zone_none;
}
