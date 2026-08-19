/* encounter_post_combat @0x8370B348 — once an encounter's combat has ended, choose and assign each
 * participating actor a post-combat behavior (investigate a corpse, celebrate, taunt, regroup, etc.).
 *
 * Overview:
 *   Phase 1: walk the encounter's actors; for every dead prop each actor holds, classify it (own kill /
 *            enemy kill / other) and add a weighted "possibility" into a 4x2 grid (row 0/1/2 = the three
 *            enemy-behavior buckets, row 3 = the leader/self bucket). Weight blends the actor's player
 *            rating, the prop distance, and the prop's age.
 *   Phase 2: if any possibility was recorded, weighted-randomly pick a first behavior; compact the grid
 *            to drop the chosen actor and, if anything remains, pick a second behavior.
 *   Phase 3: independently, pick the highest-rated non-team actor still alive and derive a behavior from
 *            its health and the encounter's kill counters; if that behavior is "search", also find the
 *            nearest OTHER actor that has an active prop for it to search toward.
 *   Phase 4: write the chosen behaviors into the actors and reset the encounter's post-combat timers.
 *
 * DEVIATIONS (disasm-verified, 0x8370B168 / 0x8370B620 / 0x8370B6A0):
 *   encounter_post_combat_add_possibility takes a float weight in f1, and the PPC ABI reserves (skips) the
 *   GPR slot that the float would have occupied — so its integer params land in r4, r6, r7 (r5 is the dead
 *   reserved slot). The decompiler lost track of this and mislabeled both call sites:
 *     - main-loop call: real args are (bucket, actor, weight, prop_iterator.index, prop->unit_index);
 *       the decompiler's phantom `v19` (r5) is the reserved slot, not an argument.
 *     - leader/self call: real args are (row-3 bucket, actor, weight, -1, -1); the decompiler's `v32`
 *       (obj_datum) is only the temp that computed the weight and happens to sit in the reserved slot.
 *   Confirmed against the callee prologue, which stores r4->actor_index, f1->weight, r6->prop_index,
 *   r7->unit_index.
 *
 * The two empty `do { ++i; } while (i < 4);` counters the decompiler emits inside the phase-3 actor walks
 * have no observable effect (no reads, no side effects) — reproduced faithfully but marked, as they are
 * almost certainly the residue of an inlined loop the optimizer collapsed.
 */

#include <stdint.h>
#include "headers/encounter_datum.h"
#include "headers/actor_definition_flags2_flags.h"
#include "headers/actor_definition.h"
#include "headers/unit_datum.h"
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/post_combat_possibility.h"
#include "headers/prop_datum.h"
#include "headers/prop_iterator.h"
#include "headers/ai_globals.h"
#include "headers/global_tag_instances.h"
#include <float.h>
#include "headers/actor_postcombat_type.h"
#include "headers/game_team.h"
#include "headers/blam_data_globals.h"


/* Maps a selected post-combat behavior index (0..3) to the actor behavior code written into the actor. */
extern const int16_t global_post_combat_translation_table[];

extern float ai_communication_get_player_rating(int unit_index, uint8_t test_line_of_sight, int *unit_index_reference, float *distance_reference);
extern void prop_iterator_new(prop_iterator *iterator, int actor_index);
extern prop_datum *prop_iterator_next(prop_iterator *iterator);
extern uint8_t encounter_post_combat_add_possibility(post_combat_possibility *possibility_array, int actor_index, float weight, int prop_index, int unit_index);
extern int16_t encounter_post_combat_select_random_behavior(const post_combat_possibility (*possibilities)[2], post_combat_possibility *selected_possibility);
extern int16_t actor_communication_team(int actor_index);
extern int prop_get_active_by_unit_index(int actor_index, int unit_index);

static void clear_post_combat_possibility(post_combat_possibility *possibility)
{
    possibility->actor_index = -1;
    possibility->weight = 0.0f;
    possibility->prop_index = -1;
    possibility->unit_index = -1;
}

void encounter_post_combat(int encounter_index)
{
    encounter_datum *encounter = DATUM_GET(encounter_data, encounter_datum, encounter_index);

    int behavior_actor = -1;                    /* v2 — highest-rated actor for the health-derived behavior */
    int16_t behavior = -1;                      /* v5 — health-derived behavior (actor_postcombat_type, written to postcombat_type) */
    char found_any_possibility = 0;             /* v6 */
    float best_team_rating = 0.0f;              /* v11 */

    /* v82: [0] = first weighted-random behavior index, [1] = second (both -1 = none). */
    int16_t selected_behavior[2] = { -1, -1 };
    int fallback = -1;                          /* v83 */
    post_combat_possibility selected[2];        /* v87, v88 */
    prop_iterator prop_iter;                    /* v84 */
    post_combat_possibility possibilities[4][2];/* v89[0..5] + v90 — one contiguous grid */

    int i, j, k;

    for ( i = 0; i < 4; i++ )
        for ( j = 0; j < 2; j++ )
            clear_post_combat_possibility(&possibilities[i][j]);

    /* Phase 1 — accumulate weighted possibilities from each actor's dead props. */
    if ( ai_globals->ai_initialized_for_map )
    {
        int actor_cursor = ( encounter_index == -1 )
            ? ai_globals->first_encounterless_actor_index
            : encounter->first_actor_index;

        while ( actor_cursor != -1 )
        {
            actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_cursor);
            int this_actor = actor_cursor;
            int unit_index = actor->meta.unit_index;
            actor_cursor = actor->meta.next_actor_index;
            actor_definition *actor_tag_definition = TAG_GET(actor_definition, actor->meta.definition_index);

            if ( unit_index != -1 )
            {
                char found_enemy = 0;
                float player_rating = ai_communication_get_player_rating(unit_index, 1u, 0, 0);

                prop_iterator_new(&prop_iter, this_actor);
                for ( prop_datum *prop = prop_iterator_next(&prop_iter);
                      prop;
                      prop = prop_iterator_next(&prop_iter) )
                {
                    if ( !prop->dead )
                        continue;

                    int16_t bucket;
                    float base_distance;
                    float extra_weight;
                    if ( prop->enemy )
                    {
                        if ( (actor_tag_definition->flags2 & (1u << _actor_definition_flags2_no_corpse_shooting_bit)) != 0 || prop->dead_ticks >= 210 )
                        {
                            bucket = 1;         /* stale/one-shot enemy kill */
                            base_distance = 5.0f;
                            extra_weight = 0.0f;
                        }
                        else
                        {
                            bucket = 0;         /* fresh enemy kill by this actor */
                            base_distance = 10.0f;
                            extra_weight = 0.69999999f;
                        }
                    }
                    else
                    {
                        bucket = 2;             /* non-enemy corpse */
                        base_distance = 9.0f;
                        extra_weight = 0.40000001f;
                    }

                    if ( prop->distance >= base_distance )
                        continue;

                    float distance_factor = base_distance / prop->distance;
                    if ( distance_factor >= 2.0f )
                        distance_factor = 2.0f;

                    float rating = player_rating <= 1.5f ? 1.5f : player_rating;

                    float age = (float)prop->dead_ticks * 0.0041666669f;
                    if ( age >= 1.0f )
                        age = 1.0f;

                    float weight = (2.0f - age) * rating * distance_factor + extra_weight;
                    if ( prop->player )
                        weight += 2.0f;

                    if ( prop->enemy )
                        found_enemy = 1;

                    if ( encounter_post_combat_add_possibility(&possibilities[bucket][0], this_actor,
                             weight, prop_iter.index, prop->unit_index) )
                        found_any_possibility = 1;
                }

                /* If this actor killed an enemy, let it also stand for a leader/self behavior. */
                if ( found_enemy )
                {
                    unit_datum *unit_object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
                    int16_t killing_spree = unit_object->unit.killing_spree_count;
                    float leader_weight = (float)killing_spree * 0.69999999f + player_rating;

                    if ( encounter_post_combat_add_possibility(&possibilities[3][0], this_actor,
                             leader_weight, -1, -1) )
                        found_any_possibility = 1;
                }
            }
        }
    }

    /* Phase 2 — pick up to two behaviors from the weighted grid. */
    if ( found_any_possibility )
    {
        int first_index = encounter_post_combat_select_random_behavior(possibilities, &selected[0]);
        selected_behavior[0] = first_index;

        if ( (uint16_t)encounter->team_index != _game_team_human || encounter->enemies_defeated >= 8 )
        {
            int selected_unit = selected[0].unit_index;
            int selected_actor = selected[0].actor_index;
            char remaining = 0;

            for ( k = 0; k < 4; k++ )
            {
                if ( k == first_index )
                {
                    for ( j = 0; j < 2; j++ )
                        clear_post_combat_possibility(&possibilities[k][j]);
                }
                else if ( possibilities[k][0].actor_index == selected_actor
                       || possibilities[k][0].unit_index == selected_unit )
                {
                    possibilities[k][0] = possibilities[k][1];
                    clear_post_combat_possibility(&possibilities[k][1]);
                }

                if ( possibilities[k][0].actor_index != -1 )
                    remaining = 1;
            }

            if ( remaining )
                selected_behavior[1] = encounter_post_combat_select_random_behavior(possibilities, &selected[1]);
        }
    }

    /* Phase 3 — health-derived behavior for the best-rated surviving non-team actor. */
    int search_actor = -1;                      /* v63 */
    int search_prop = -1;                       /* v64 */

    if ( (uint16_t)encounter->team_index != _game_team_human || encounter->enemies_defeated >= 4 )
    {
        char want_search = 0;                   /* v51 */
        int best_actor = -1;                    /* v52 */

        if ( ai_globals->ai_initialized_for_map )
        {
            int actor_cursor = ( encounter_index == -1 )
                ? ai_globals->first_encounterless_actor_index
                : encounter->first_actor_index;

            while ( actor_cursor != -1 )
            {
                actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_cursor);
                int this_actor = actor_cursor;
                int unit_index = actor->meta.unit_index;
                actor_cursor = actor->meta.next_actor_index;

                if ( unit_index != -1 )
                {
                    int16_t dead_counter = 0;   /* v57 — empty decompiler counter, no effect */
                    do
                        ++dead_counter;
                    while ( dead_counter < 4 );

                    float rating = ai_communication_get_player_rating(unit_index, 1u, 0, 0);
                    if ( !actor_communication_team(this_actor) && rating > 2.0f && rating > best_team_rating )
                    {
                        best_actor = this_actor;
                        best_team_rating = rating;
                    }
                }
            }
        }

        behavior_actor = best_actor;
        if ( best_actor != -1 )
        {
            actor_datum *chosen = DATA_ARRAY_ELEMENT(actor_data, actor_datum, best_actor);
            float health = chosen->input.body_vitality;

            if ( health >= 0.5f || (chosen->emotions.original_body_vitality - health) <= 0.30000001f )
            {
                int16_t living = encounter->current_count;
                if ( living == 1 && encounter->prebattle_living_count > 1 )
                {
                    behavior = actor_postcombat_speak_alone;
                }
                else
                {
                    if ( living >= 2 )
                    {
                        int16_t threshold = 2;
                        if ( living <= 2 )
                            threshold = encounter->current_count;

                        if ( encounter->prebattle_living_count >= threshold + living )
                        {
                            behavior = actor_postcombat_speak_massacre;
                            goto apply_behaviors_check;
                        }
                        if ( living >= encounter->prebattle_living_count - 1 )
                        {
                            behavior = actor_postcombat_speak_triumph;
                            goto apply_behaviors_check;
                        }
                    }
                    if ( health > 0.80000001f )
                        behavior = actor_postcombat_speak_unscathed;
                }
            }
            else
            {
                behavior = actor_postcombat_speak_wounded;
                want_search = 1;
            }

apply_behaviors_check:
            /* If the behavior is a corpse search, locate the nearest OTHER actor holding an active prop. */
            if ( want_search )
            {
                float best_distance = FLT_MAX;  /* v65 */

                if ( ai_globals->ai_initialized_for_map )
                {
                    int actor_cursor = ( encounter_index == -1 )
                        ? ai_globals->first_encounterless_actor_index
                        : encounter->first_actor_index;

                    while ( actor_cursor != -1 )
                    {
                        actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_cursor);
                        int this_actor = actor_cursor;
                        actor_cursor = actor->meta.next_actor_index;

                        if ( actor->meta.unit_index != -1 )
                        {
                            int16_t dead_counter = 0;   /* v69 — empty decompiler counter, no effect */
                            do
                                ++dead_counter;
                            while ( dead_counter < 4 );

                            if ( this_actor != best_actor )
                            {
                                float dx = actor->input.position.head_position.__s1.y - chosen->input.position.head_position.__s1.y;
                                float dy = actor->input.position.head_position.__s1.z - chosen->input.position.head_position.__s1.z;
                                float dz = actor->input.position.head_position.__s1.x - chosen->input.position.head_position.__s1.x;
                                float distance_squared = dz * dz + (dy * dy + dx * dx);

                                /* As shipped: best_distance stores a squared distance yet is compared
                                 * squared again — reproduced verbatim. */
                                if ( best_distance == FLT_MAX
                                  || distance_squared < best_distance * best_distance )
                                {
                                    int active_prop = prop_get_active_by_unit_index(this_actor,
                                        chosen->meta.unit_index);
                                    if ( active_prop != -1 )
                                    {
                                        best_distance = distance_squared;
                                        search_actor = this_actor;
                                        search_prop = active_prop;
                                    }
                                }
                            }
                        }
                    }
                }

                if ( search_actor != -1 )
                    goto apply_behaviors;
            }
        }
    }

    search_prop = fallback;
    search_actor = fallback;

apply_behaviors:
    /* Phase 4 — commit the chosen behaviors and reset the encounter's post-combat state. */
    for ( i = 0; i < 2; i++ )
    {
        if ( selected_behavior[i] != -1 && selected[i].actor_index != -1 )
        {
            actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, selected[i].actor_index);
            actor->external_orders.postcombat_type = global_post_combat_translation_table[selected_behavior[i]];
            actor->external_orders.postcombat_prop_index = selected[i].prop_index;
        }
    }

    if ( behavior != -1 && behavior_actor != -1 )
    {
        actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, behavior_actor);
        actor->external_orders.postcombat_type = behavior;
        actor->external_orders.postcombat_prop_index = -1;

        if ( search_actor != -1 )
        {
            actor_datum *searcher = DATA_ARRAY_ELEMENT(actor_data, actor_datum, search_actor);
            searcher->external_orders.postcombat_type = actor_postcombat_run_to;
            searcher->external_orders.postcombat_prop_index = search_prop;
        }
    }

    encounter->post_combat_delay = 0;
    encounter->enemies_defeated = 0;
    encounter->post_combat_delay_timer = 120;
    encounter->post_combat = 1;
}
