/* actor_action_handle_lost_contact @0x837F5C38 — decide what an actor does after it loses contact with its
 * target: uncover, search, wait, guard the last known position, or fall back to a default/idle state. It
 * gathers the actor's pursuit "desires" (from its type, its encounter group, and its assigned orphan prop),
 * resolves which pursuit options are currently allowed, then tries each candidate action in priority order
 * (uncover target -> search target -> uncover/search a selected pursuit firing position -> wait), emitting the
 * appropriate AI communication events, and finally settles on a post-combat or find-position guard. Returns 1
 * if the actor's action changed (or a guard was forced at the end).
 *
 * 2026-07-14 fully typed. Deviations, all verified:
 *   - The decompiler lost the spilled output-pointer arguments of encounter_determine_pursuit_availability and
 *     actor_action_determine_pursuit_options (shown as uninitialised v32..v37). Recovered from disasm
 *     0x837F5EAC-0x837F5F34: the four trailing pointers of the availability call are &allow_pursuit_search,
 *     &controlling_group_pursuit, &controlled_by_group_pursuit, &wait_after_pursuit; the pursuit-options call
 *     passes &allow_target_uncover as its `must_charge` slot (matching the callee's pointer-typed param) then
 *     &allow_indefinite_uncover, &allow_target_search, &allow_pursuit, &allow_pursuit_search, &wait_after_pursuit.
 *   - The disasm stores only 13 of pursuit-options' 14 args; the 14th (allow_waiting) is left as a stale
 *     outgoing slot and the callee documents param 14 as unused, so a harmless in-scope pointer is passed here.
 *   - The __chkstk stack-probe (RtlCheckStack12 + back-chain fixup the decompiler printed around the large
 *     firing_position_evaluation_context frame) is compiler boilerplate and is omitted; memset is the real init. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/action_state_data.h"
#include "headers/action_specification.h"
#include "headers/action_class.h"
#include "headers/firing_position.h"
#include "headers/firing_position_evaluation_context.h"
#include "headers/global_tag_instances.h"
#include "headers/actor_datum.h"
#include "headers/actor_definition.h"
#include "headers/encounter_datum.h"
#include "headers/prop_datum.h"
#include "headers/uncover_state_data.h"
#include "headers/search_state_data.h"
#include "headers/guard_state_data.h"
#include "headers/actor_action.h"
#include "headers/actor_postcombat_type.h"
#include "headers/actor_combat_status.h"
#include "headers/actor_mode.h"
#include "headers/firing_point_evaluation_mode.h"
#include "headers/pursuit_location_type.h"
#include "headers/ai_communication_type.h"
#include "headers/blam_data_globals.h"

typedef struct wait_state_data wait_state_data;
typedef struct path_state path_state;
typedef struct ai_information_data ai_information_data;

#include "headers/uncover_state_data.h"
#include "headers/search_state_data.h"
#include "headers/guard_state_data.h"
#include "headers/search_state_data.h"
extern action_specification global_action_functions[];

extern int16_t actor_type_get_when_to_search_at_target(int16_t actor_type);
extern int16_t actor_type_get_when_to_pursue(int16_t actor_type);
extern int16_t actor_type_get_when_to_search_pursuit(int16_t actor_type);
extern uint8_t actor_type_get_pursuit_controller(int16_t actor_type);
extern void encounter_modify_pursuit_desires(uint16_t encounter_index, int16_t squad_index, uint8_t *pursue_tenacious, int16_t *group_pursuit_restriction, uint8_t *group_pursuit_controller, int16_t *desired_target_search, int16_t *desired_pursuit, int16_t *desired_pursuit_search);
extern void encounter_determine_pursuit_availability(int encounter_index, int actor_index, int16_t group_pursuit_restriction, uint8_t is_pursuit_controller, uint8_t *allow_target_uncover, uint8_t *allow_indefinite_target_uncover, uint8_t *allow_target_search, uint8_t *allow_pursuit, uint8_t *allow_pursuit_search, uint8_t *controlling_group_pursuit, uint8_t *controlled_by_group_pursuit, uint8_t *wait_after_pursuit);
extern void actor_action_determine_pursuit_options(int actor_index, int16_t desire_target_search, int16_t desire_pursuit, int16_t desire_pursuit_search, uint8_t controlling_group_pursuit, uint8_t controlled_by_group_pursuit, uint8_t stand_down, uint8_t *must_charge, uint8_t *allow_target_uncover, uint8_t *allow_indefinite_target_uncover, uint8_t *allow_target_search, uint8_t *allow_pursuit, uint8_t *allow_pursuit_search, uint8_t *allow_waiting);
extern uint8_t action_uncover_setup_target(int actor_index, uint8_t able_to_search, uncover_state_data *state_data);
extern uint8_t action_uncover_setup_pursuit(uint16_t actor_index, int16_t firing_position_index, uncover_state_data *state_data);
extern uint8_t action_search_setup_target(int actor_index, uint8_t must_charge, search_state_data *state_data);
extern uint8_t action_search_setup_pursuit(uint16_t actor_index, int16_t firing_position_index, uint8_t tenacious, search_state_data *state_data);
extern int action_search_setup_undirected(uint16_t actor_index, uint8_t tenacious, search_state_data *state_data);
extern uint8_t action_wait_setup(uint16_t actor_index, uint8_t actively_searching, wait_state_data *state_data);
extern uint8_t action_guard_setup_postcombat(int actor_index, guard_state_data *state_data);
extern uint8_t action_guard_setup_find_position(uint16_t actor_index, int16_t delay_timer, guard_state_data *state_data);
extern void actor_action_change(int actor_index, int new_action_type, action_state_data *new_action_data);
extern uint8_t actor_action_set_default_state(int actor_index, int16_t override_state);
extern void actor_perception_tried_to_uncover(int actor_index, int prop_index);
extern void actor_perception_tried_to_search(int actor_index, int prop_index);
extern void actor_perception_abandoned_search(int actor_index, int prop_index);
extern int actor_target_unit_index(uint16_t actor_index);
extern int16_t actor_select_firing_position(int actor_index, firing_position_evaluation_context *evaluation_context,
        firing_position *best_firing_position, int *current_owner, path_state *area_path_state,
        uint8_t *area_path_state_valid);
extern unsigned int actor_get_firing_position_group(uint16_t actor_index, int16_t evaluation_mode, int16_t group_selection_mode);
extern uint8_t encounter_mark_examined_pursuit_position(int encounter_index, int actor_index, int16_t firing_position_index, int history_start_time);
extern void ai_communication_event(int16_t communication_type, int subject_unit_index, int cause_unit_index, int16_t hostility, int16_t damage_type, int16_t information_type, ai_information_data *information_data);
extern void *memset(void *destination, int value, unsigned int size);

uint8_t actor_action_handle_lost_contact(int actor_index)
{
    actor_datum *actor = DATUM_GET(actor_data, actor_datum, actor_index);
    actor_firing_position_data *actor_firing_positions = &actor->firing_positions;

    /* the actor definition tag (a prior pass mislabeled this the variant definition; +22 ints is
     * meta.definition_index, and the +852/+854 reads below are pursuit.* fields of actor_definition). */
    actor_definition *character = TAG_GET(actor_definition, actor->meta.definition_index);

    encounter_datum *encounter = (actor->meta.encounter_index == -1)
                          ? nullptr
                          : DATUM_GET(encounter_data, encounter_datum, actor->meta.encounter_index);

    uint8_t changed = 0;
    uint8_t should_return_to_idle = 0;
    char wants_postcombat_guard = 0;
    action_state_data action_data; /* v57 — the single action working-state buffer reused for every setup */

    /* return to idle when the encounter is standing down and combat has fully wound down. */
    if (encounter && encounter->stand_down && actor->state.combat_status <= _actor_combat_status_investigate && !actor->state.artificial_combat_status)
        should_return_to_idle = actor->state.suspicion_combat_status == _actor_combat_status_none;
    /* fall to a post-combat guard when external orders demand it and combat is winding down. */
    if (actor->external_orders.postcombat_type > actor_postcombat_none && actor->state.combat_status <= _actor_combat_status_investigate && !actor->state.suspicion_combat_status)
        wants_postcombat_guard = 1;
    /* already resolved this tick if the current mode is non-combat and the action has no class. */
    if (actor->state.mode < _actor_mode_combat && !global_action_functions[actor->state.action].action_class)
        changed = 1;

    if (!changed && !actor->input.vehicle_passenger && !wants_postcombat_guard && !should_return_to_idle
        && actor->state.combat_status >= _actor_combat_status_investigate)
    {
        prop_datum *orphan = (actor->target.target_prop_index == -1)
                           ? nullptr
                           : DATUM_GET(prop_data, prop_datum, actor->target.target_prop_index);

        uint8_t allow_target_uncover = 0;
        uint8_t allow_indefinite_uncover = 0;
        uint8_t allow_target_search = 0;
        uint8_t allow_pursuit = 0;
        uint8_t allow_pursuit_search = 0;
        uint8_t pursue_tenacious = 0;
        uint8_t wait_after_pursuit = 0;
        uint8_t controlling_group_pursuit;
        uint8_t group_pursuit_controller;
        uint8_t controlled_by_group_pursuit;
        int16_t when_to_pursue;
        int16_t when_to_search_pursuit;
        int16_t group_pursuit_restriction;
        int search_scratch[4];
        int16_t when_to_search_at_target;

        /* skip desire evaluation when a live orphan already directs the search (abandoned_search). */
        if (!orphan || !orphan->abandoned_search)
        {
            when_to_search_at_target = actor_type_get_when_to_search_at_target((uint16_t)actor->meta.type);
            when_to_pursue = actor_type_get_when_to_pursue((uint16_t)actor->meta.type);
            when_to_search_pursuit = actor_type_get_when_to_search_pursuit((uint16_t)actor->meta.type);
            group_pursuit_controller = actor_type_get_pursuit_controller((uint16_t)actor->meta.type);
            controlling_group_pursuit = 0;
            group_pursuit_restriction = 0;
            controlled_by_group_pursuit = 0;
            if (orphan)
            {
                allow_indefinite_uncover = 1;
                allow_target_uncover = 1;
            }
            int encounter_index = actor->meta.encounter_index;
            allow_pursuit = 1;
            allow_target_search = 1;
            allow_pursuit_search = 1;
            if (encounter_index != -1)
            {
                encounter_modify_pursuit_desires(encounter_index, (uint16_t)actor->meta.squad_index,
                                                 &pursue_tenacious, &group_pursuit_restriction,
                                                 &group_pursuit_controller, &when_to_search_at_target,
                                                 &when_to_pursue, &when_to_search_pursuit);
                if (actor->meta.swarm) /* swarm actors never restrict pursuit */
                {
                    allow_target_search = 1;
                    allow_indefinite_uncover = 1;
                    allow_target_uncover = 1;
                    allow_pursuit_search = 1;
                    allow_pursuit = 1;
                }
                else
                {
                    encounter_determine_pursuit_availability(
                        encounter_index, actor_index, group_pursuit_restriction, group_pursuit_controller,
                        &allow_target_uncover, &allow_indefinite_uncover, &allow_target_search, &allow_pursuit,
                        &allow_pursuit_search, &controlling_group_pursuit, &controlled_by_group_pursuit,
                        &wait_after_pursuit);
                }
            }
            actor_action_determine_pursuit_options(
                actor_index, when_to_search_at_target, when_to_pursue, when_to_search_pursuit,
                controlling_group_pursuit, should_return_to_idle, actor->emotions.forced_to_charge, &allow_target_uncover,
                &allow_indefinite_uncover, &allow_target_search, &allow_pursuit, &allow_pursuit_search,
                &wait_after_pursuit,
                &controlled_by_group_pursuit); /* param 14 (allow_waiting): callee-unused; disasm omits it */
        }

        /* reset the examined-position bookkeeping if the tracked orphan changed. */
        int orphan_prop_index = actor->target.target_prop_index;
        if (actor_firing_positions->pursuit_prop_index != orphan_prop_index)
        {
            actor_firing_positions->pursuit_prop_index = orphan_prop_index;
            actor_firing_positions->pursuit_positions_count = 0;
            actor_firing_positions->pursuit_fired_at_orphan = 0;
            actor_firing_positions->pursuit_communicated_lost_contact = 0;
        }

        if (allow_target_uncover
            && action_uncover_setup_target(actor_index, allow_target_search, &action_data.___u0.uncover))
        {
            actor_action_change(actor_index, actor_action_uncover, &action_data);
            changed = 1;
        }

        if (!changed)
        {
            actor_perception_tried_to_uncover(actor_index, actor->target.target_prop_index);
            if (allow_target_search
                && action_search_setup_target(actor_index, actor->emotions.forced_to_charge, &action_data.___u0.search))
            {
                actor_action_change(actor_index, actor_action_search, &action_data);
                changed = 1;
            }
            uint8_t changed_snapshot = changed;
            if (!changed)
            {
                actor_perception_tried_to_search(actor_index, actor->target.target_prop_index);
                if (actor_firing_positions->pursuit_fired_at_orphan && !actor_firing_positions->pursuit_communicated_lost_contact)
                {
                    int target_unit = actor_target_unit_index(actor_index);
                    ai_communication_event(_ai_communication_lost_contact, actor->meta.unit_index, target_unit, -1, -1, -1, nullptr);
                    actor_firing_positions->pursuit_communicated_lost_contact = 1;
                }

                int16_t firing_position_index = -1;
                char pursue_from_guard = 0;
                uint8_t search_started;
                if (!allow_pursuit)
                    goto check_examined;
                actor->state.searching = 1;
                if (actor->meta.swarm) /* swarm: undirected search only */
                {
                    if (!allow_pursuit_search)
                        goto examine_pursuit_position;
                    search_started = action_search_setup_undirected(actor_index, pursue_tenacious,
                                                                    &action_data.___u0.search);
                }
                else
                {
                    /* directed: take a scripted pursuit position from the current guard action, else select one. */
                    if (actor->state.action == actor_action_uncover && allow_pursuit_search
                        && actor->state.action_data.___u0.uncover.pursuit_location.type == _pursuit_location_position)
                    {
                        firing_position_index = actor->state.action_data.___u0.uncover.pursuit_location.firing_position_index;
                        pursue_from_guard = 1;
                    }
                    if (firing_position_index == -1)
                    {
                        int16_t examine_threshold = (actor->external_orders.pursuit_group_prop_index == -1)
                                                        ? character->pursuit.uncoordinated_pursuit_positions
                                                        : character->pursuit.coordinated_pursuit_positions;
                        if (pursue_tenacious || actor_firing_positions->pursuit_prop_index != actor->target.target_prop_index
                            || actor_firing_positions->pursuit_positions_count < examine_threshold)
                        {
                            firing_position_evaluation_context evaluation_context;
                            memset(&evaluation_context, 0, sizeof(evaluation_context));
                            int tracked_orphan = actor->target.target_prop_index;
                            evaluation_context.evaluation_data.___u0.pursue.orphan_prop_index = tracked_orphan;
                            evaluation_context.evaluation_mode = _firing_point_evaluation_mode_pursue;
                            evaluation_context.evaluation_data.___u0.pursue.last_perceived_time =
                                orphan ? orphan->last_perceived_time : -1;
                            evaluation_context.evaluation_data.___u0.pursue.tenacious = pursue_tenacious;
                            evaluation_context.find_path_direction_from_target = tracked_orphan != -1;
                            evaluation_context.allowed_position_mask =
                                actor_get_firing_position_group(actor_index, _firing_point_evaluation_mode_pursue, 0);
                            evaluation_context.maximum_search_range = 20.0f;
                            firing_position best_firing_position;
                            firing_position_index = actor_select_firing_position(
                                actor_index, &evaluation_context, &best_firing_position, search_scratch,
                                (path_state *)&allow_pursuit_search, &controlled_by_group_pursuit);
                        }
                    }
                    if (firing_position_index == -1)
                        goto examine_pursuit_position;

                    if (!pursue_from_guard
                        && action_uncover_setup_pursuit(actor_index, firing_position_index,
                                                        &action_data.___u0.uncover))
                    {
                        actor_action_change(actor_index, actor_action_uncover, &action_data);
                        changed = 1;
                    }
                    if (changed || !allow_pursuit_search)
                        goto examine_pursuit_position;
                    search_started = action_search_setup_pursuit(actor_index, firing_position_index,
                                                                 pursue_tenacious, &action_data.___u0.search);
                }
                /* shared tail for both the swarm-undirected and directed-pursuit search attempts. */
                if (search_started)
                {
                    actor_action_change(actor_index, actor_action_search, &action_data);
                    changed = 1;
                }

            examine_pursuit_position: /* LABEL_73 */
                changed_snapshot = changed;
                if (!changed)
                    goto try_wait;
                /* committed to a pursuit position: record it as examined and announce the search. */
                {
                    int orphan_last_perceived = orphan ? orphan->last_perceived_time : -1;
                    int pursuit_encounter = actor->meta.encounter_index;
                    if (pursuit_encounter != -1
                        && encounter_mark_examined_pursuit_position(pursuit_encounter, actor_index,
                                                                    firing_position_index, orphan_last_perceived))
                    {
                        if (!actor_firing_positions->pursuit_positions_count)
                            ai_communication_event(_ai_communication_search_start, actor->meta.unit_index, -1, -1, -1, -1, nullptr);
                        ++actor_firing_positions->pursuit_positions_count;
                    }
                }
            check_examined: /* LABEL_81 */
                if (changed_snapshot)
                    goto commit;
            try_wait: /* LABEL_82 — nothing committed: announce abandonment, then try a wait action. */
                if (actor_firing_positions->pursuit_positions_count > 0)
                {
                    int subject_unit = actor->meta.unit_index;
                    if (subject_unit != -1)
                        ai_communication_event(_ai_communication_search_abandon, subject_unit, -1, -1, -1, -1, nullptr);
                }
                if (!changed_snapshot && !actor->meta.swarm && wait_after_pursuit
                    && action_wait_setup(actor_index, allow_pursuit, &action_data.___u0.wait))
                {
                    actor_action_change(actor_index, actor_action_wait, &action_data);
                    changed = 1;
                }
            }
        }
    }

commit:
    if (!changed && wants_postcombat_guard)
    {
        if (actor->state.action == actor_action_guard && actor->state.action_data.___u0.guard.post_combat)
        {
            changed = 1;
        }
        else if (action_guard_setup_postcombat(actor_index, &action_data.___u0.guard))
        {
            actor_action_change(actor_index, actor_action_guard, &action_data);
            changed = 1;
        }
    }

    if (!changed && should_return_to_idle)
        changed = actor_action_set_default_state(actor_index, -1);

    if (changed
        || global_action_functions[actor->state.action].action_class == _action_class_passive)
        return changed;

    /* still nothing: force a find-position guard, delaying if the actor is not already searching/waiting. */
    int find_position_delay = 0;
    int current_action = actor->state.action;
    if ((current_action != actor_action_search || actor->state.action_data.___u0.search.search_failed) && current_action != actor_action_wait)
        find_position_delay = 90;
    actor_perception_abandoned_search(actor_index, actor->target.target_prop_index);
    action_guard_setup_find_position(actor_index, find_position_delay, &action_data.___u0.guard);
    actor_action_change(actor_index, actor_action_guard, &action_data);
    return 1;
}
