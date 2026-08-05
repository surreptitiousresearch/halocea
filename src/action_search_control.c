/* action_search_control @0x83823450 — per-tick control update for an actor's "search" action. If the
 * actor was told to give up early (+1284), switches straight to search-mode 0 ("done", sub-mode 0).
 * Otherwise, once a per-tick timer (a window derived from a definition-tag-driven duration, floored at
 * 90 ticks) elapses since the last update, transitions: if a search cue is pending (+164 word), either
 * commits to investigating it (mode 3/sub-mode 3, copying the cue's position into the actor's search
 * target) when the cue is "confirmed" (== 1), or moves to sub-mode 2 otherwise; if no cue is pending,
 * falls back to sub-mode 1 ("idle search"). Always finishes by setting search-phase to 3, and — only
 * while no search cue is pending — recomputes whether the actor has "looked long enough" (+1108) against
 * a definition-flag-dependent threshold (5 or 6 ticks) using the actor's grenade-readiness timer field.
 * Finally mirrors a state byte (+159) into a 3-slot history-ish block (+1062..+1064) and marks +530 done.
 *
 * Fields are accessed through typed actor_datum members: the "give up search" flag is control.moving
 * (control+152), and the "looked long enough" comparison reads target.target_type (target+616,
 * DB-verified). This function's tag lookup is against the actor's own definition (meta.definition_index),
 * matching actor_action_handle_active_cover_seeking.c's convention. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/search_state_data.h"
#include "headers/direction_specification_type.h"
#include "headers/primary_look_priority.h"
#include "headers/idle_look_type.h"
#include "headers/actor_definition_flags.h"
#include "headers/pursuit_location_type.h"
#include "headers/blam_data_globals.h"


void action_search_control(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    const unsigned int *definition_tag = TAG_GET(const unsigned int, actor->meta.definition_index);

    /* "give up search" flag: control.moving (control+152). */
    if ( actor->control.moving )
    {
        actor->orders.look.primary_priority = _primary_priority_facing;
        actor->orders.look.primary_direction.type = _direction_specification_movement;
    }
    else
    {
        int elapsed = actor->state.action_data.___u0.search.search_desired_time;
        int last_update = actor->state.action_data.___u0.search.search_remaining_time;
        int window = elapsed / 3;
        if ( window <= 90 )
            window = 90;

        uint8_t transitioned = 0;

        if ( elapsed - last_update < window )
        {
            /* pursuit_location.type: _pursuit_location_target(0) vs _pursuit_location_position(1). */
            int16_t *search_cue_ref = &actor->state.action_data.___u0.search.pursuit_location.type;

            if ( *search_cue_ref )
            {
                if ( *search_cue_ref == _pursuit_location_position )
                {
                    actor->orders.look.primary_priority = _primary_priority_facing;
                    actor->orders.look.primary_direction.type = _direction_specification_point;

                    /* copy the search cue's position into the actor's committed search target. */
                    /* DEVIATION: decompiler word-punned the 3x lwz/stw real_point3d copy
                     * (0x83823508..18) across the union arms; plain assignment into the point arm. */
                    actor->orders.look.primary_direction.___u1.point = actor->state.action_data.___u0.search.pursuit_location.position;

                    transitioned = 1;
                }
            }
            else
            {
                actor->orders.look.primary_priority = _primary_priority_facing;
                actor->orders.look.primary_direction.type = _direction_specification_target;
                transitioned = 1;
            }

            /* *search_cue_ref not 0 and not 1: no explicit case in the original — transitioned stays 0,
             * matching the shipped control flow's implicit "fall through to sub-mode 1" path below. */
        }

        if ( !transitioned )
            actor->orders.look.primary_priority = _primary_priority_face_360;
    }

    actor->orders.look.idle_look_type = _idle_look_searching;

    /* pending search cue (pursuit_location.type), reused as the "no cue pending" gate below. */
    if ( !actor->state.action_data.___u0.search.pursuit_location.type )
    {
        int threshold = 5;
        /* actor_definition.flags@0 bit 4 = shoot_at_targets_last_location. */
        if ( (*definition_tag & (1u << _actor_definition_shoot_at_targets_last_location_bit)) == 0 )
            threshold = 6;

        /* "looked long enough" flag (orders.combat.shoot_at_target) compared against
         * target.target_type (target+616, DB-verified). */
        actor->orders.combat.shoot_at_target = actor->target.target_type >= threshold;
    }

    char state_byte = actor->state.action_data.___u0.search.sneaking;
    actor->orders.move.stationary_crouch = state_byte;
    actor->orders.move.panicked = 0;
    actor->orders.move.dive_into_cover = 1;
    actor->orders.move.emerge_from_cover = 0;
    actor->orders.move.moving_crouch = state_byte;
}
