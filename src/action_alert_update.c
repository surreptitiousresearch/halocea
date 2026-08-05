/* action_alert_update @0x83826558 — per-tick update for the actor "alert" action. No-op if the actor is
 * dormant or has no active alert target (action_data +6 as WORD == 0xFFFF). Once the actor has reached its
 * stored alert destination (action_data +12, a real_point3d) — either via actor_path_at_destination or by
 * being within 0.5 units of it — ticks down a countdown timer (action_data +2) and, the first time the
 * "play startle animation" flag (action_data +10) is set, starts the actor's unit on its alert-animation
 * reference (action_data +40, an index into the scenario's ai_animation_references block; falling back to
 * the unit's tag-defined default animation graph if that reference's animation index is -1), then clears
 * the flag.
 *
 * The action_data fields (+2/+6/+10/+12/+40) are resolved to the DB-named alert_state_data arm; the
 * ai_animation_reference record is typed — the actor_datum/actor_meta_data/actor_position_data
 * chain resolves everything else. The object_header_data dereference for the unit's tag definition here
 * compiles to a literal `3*idx+2` — algebraically the same established `idx + ((2*idx)&0x1FFFE) + 2` idiom,
 * since for any 16-bit idx that mask is a no-op (2*idx never exceeds 0x1FFFE). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/alert_state_data.h"
#include "headers/scenario.h"
#include "headers/ai_animation_reference_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/_object_definition.h"
#include "headers/blam_data_globals.h"


extern uint8_t actor_path_at_destination(uint16_t actor_index);
extern uint8_t unit_start_user_animation(int unit_index, int animation_graph_index, const char *animation_name, uint8_t interpolate);

void action_alert_update(uint16_t actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    alert_state_data *alert = &actor->state.action_data.___u0.alert;

    if ( actor->meta.dormant || (uint16_t)alert->target_move_position_index == 0xFFFF )
        return;

    float dx = alert->target_position_definition.position.x - actor->input.position.body_position.n[0];
    float dy = alert->target_position_definition.position.y - actor->input.position.body_position.n[1];
    float dz = alert->target_position_definition.position.z - actor->input.position.body_position.n[2];

    if ( !actor_path_at_destination(actor_index) && dx * dx + dy * dy + dz * dz >= 0.25f )
        return;

    int16_t countdown = alert->wait_ticks;
    if ( countdown > 0 )
        alert->wait_ticks = countdown - 1;

    if ( alert->must_play_animation )
    {
        int16_t animation_reference_index = alert->target_position_definition.animation_reference_index;
        if ( animation_reference_index != -1 )
        {
            ai_animation_reference_definition *animation_reference =
                &((ai_animation_reference_definition *)global_scenario->ai_animation_references.address)[animation_reference_index];
            int animation_graph_index = animation_reference->animation_graph.index;

            if ( animation_graph_index == -1 )
            {
                object_datum *unit_object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, actor->meta.unit_index)->datum;
                _object_definition *unit_definition = TAG_GET(_object_definition, unit_object->definition_index);  /* DEVIATION: restored missing *(T**) deref */
                animation_graph_index = unit_definition->animation_graph.index;
            }
            /* recovered: the (const char *) cast was punning the whole record — animation_name is
             * ai_animation_reference_definition's first member (char[32] @ +0), which is what the
             * DB prototype's `const char *animation_name` parameter actually wants.
             * The trailing 1 is the `interpolate` bool, not an enum (enum_oracle: no candidate). */
            unit_start_user_animation(actor->meta.unit_index, animation_graph_index,
                animation_reference->animation_name, 1u);
        }

        alert->must_play_animation = 0;
    }
}
