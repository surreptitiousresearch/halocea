/* encounter_update_follow @0x8370C418 — migrate an encounter's squads between firing positions to
 * "follow" a set of target units (the players, a specific unit, or the actors of a referenced AI index).
 *
 * Phase 1: build the follow-target unit list (up to 8) from encounter->follow_target_type.
 * Phase 2: for each squad flagged automatic-migration in the scenario, accumulate the squads-present /
 *   squads-migrating / squads-with-living bit vectors, per-squad allowed firing-group masks, and the
 *   total living count.
 * Phase 3: pick the closest follow-target origin, compute each firing-position group's nearest squared
 *   distance to it, then compare the migrating squad whose allowed groups come closest (nearest_squad)
 *   against the populated squad whose allowed groups are farthest (farthest_squad); if they differ by
 *   more than the encounter's follow distance, migrate every actor into nearest_squad.
 *
 * DEVIATION: the decompiler fuses the target list, per-target min distances, target origins, per-group
 * distances and per-squad masks into one overlaid stack int array (v90[110]), and packs the three squad
 * bit vectors into two __int64s plus a punned iterator slot ("local variable allocation has failed"
 * class); untangled here into separately typed locals per the disasm store offsets. Behavior-identical.
 * PPC `__fsqrts` is written as sqrtf (marked at the call sites).
 */

#include <stdint.h>
#include <math.h>
#include "headers/encounter_datum.h"
#include "headers/ai_follow_target_type.h"
#include "headers/squad_flags.h"
#include "headers/encounter_definition.h"
#include "headers/squad_definition.h"
#include "headers/firing_position_definition.h"
#include "headers/firing_position_group.h"
#include "headers/firing_position_group_index.h"
#include "headers/actor_datum.h"
#include "headers/ai_index_actor_iterator.h"
#include "headers/data_iterator.h"
#include "headers/squad_datum.h"
#include "headers/platoon_datum.h"
#include "headers/ai_globals.h"
#include "headers/real_point3d.h"
#include "headers/scenario.h"
#include "headers/data_array.h"
#include "headers/object_type.h"
#include "headers/player_datum.h"
#include "headers/bit_vector.h"
#include "headers/blam_data_globals.h"

/* encounter_data / actor_data / player_data come from blam_data_globals.h (data_array *) */
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern void actor_change_encounter(int actor_index, int encounter_index, int16_t squad_index);
extern void ai_index_actor_iterator_new(unsigned int ai_index, ai_index_actor_iterator *iterator);
extern actor_datum *ai_index_actor_iterator_next(ai_index_actor_iterator *iterator);

void encounter_update_follow(int encounter_index)
{
    encounter_datum *encounter;
    encounter_definition *encounter_def;
    int follow_mode;
    int best_target_unit;                     /* chosen follow-target unit object index */
    int16_t target_count;
    real_point3d follow_origin;
    uint32_t squads_present[2];               /* squads with automatic migration this update */
    uint32_t squads_migrating[2];             /* squads whose firing set wants a migration target */
    uint32_t squads_with_living[2];           /* migrating squads with a living actor */
    int target_units[8];                      /* follow-target unit object indices */
    float target_min_d2[8];                   /* per-target nearest actor squared distance */
    real_point3d target_origins[8];
    float group_min_d2[NUMBER_OF_FIRING_POSITION_GROUP_INDICES]; /* per letter group, from follow_origin */
    int squad_position_masks[64];             /* per squad: allowed firing-position group mask */

    encounter = DATUM_GET(encounter_data, encounter_datum, encounter_index);
    encounter_def = (encounter_definition *)global_scenario->ai_encounters.address + (uint16_t)encounter_index;
    follow_mode = (uint16_t)encounter->follow_target_type;
    best_target_unit = -1;
    target_count = 0;

    switch (follow_mode)
    {
        case _follow_target_players:
        {
            data_iterator player_iter;
            player_datum *player;
            data_iterator_new(&player_iter, player_data);
            for (player = data_iterator_next(&player_iter); player;
                 player = data_iterator_next(&player_iter))
            {
                if (player->unit_index != -1 && target_count < 8)
                    target_units[target_count++] = player->unit_index;
            }
            break;
        }
        case _follow_target_unit:
            if (object_try_and_get_and_verify_type(encounter->___u43.follow_target_unit_index, object_mask_unit))
            {
                target_count = 1;
                target_units[0] = encounter->___u43.follow_target_unit_index;
            }
            else
            {
                encounter->___u43.follow_target_unit_index = -1;
            }
            break;
        case _follow_target_ai:
        {
            int ai_index = encounter->___u43.follow_target_ai_index;
            if (ai_index != -1)
            {
                ai_index_actor_iterator actor_iterator;
                actor_datum *actor;
                ai_index_actor_iterator_new(ai_index, &actor_iterator);
                for (actor = ai_index_actor_iterator_next(&actor_iterator); actor;
                     actor = ai_index_actor_iterator_next(&actor_iterator))
                {
                    if (target_count >= 8)
                        break;
                    target_units[target_count++] = actor->meta.unit_index;
                }
            }
            break;
        }
    }

    if (target_count <= 0)
        return;

    int16_t squad_count = encounter->squad_count;
    int total_living_count = 0;
    int reachable_group_mask = 0;
    squads_present[0] = squads_present[1] = 0;
    squads_migrating[0] = squads_migrating[1] = 0;
    squads_with_living[0] = squads_with_living[1] = 0;

    if (squad_count > 0)
    {
        squad_definition *squad_defs = (squad_definition *)encounter_def->squads.address;
        squad_datum *squads = squad_array;
        platoon_datum *platoons = platoon_array;
        for (int16_t s = 0; s < squad_count; ++s)
        {
            squad_definition *squad_def = &squad_defs[s];
            if ((squad_def->flags & (1u << _squad_automatic_migration_bit)) != 0)
            {
                squad_datum *squad = &squads[(int16_t)((uint16_t)encounter->squad_base + s)];
                BIT_VECTOR_SET_FLAG(squads_present, s);
                int current_count = squad->current_count;
                total_living_count += current_count;
                if (squad->automatic_migration_target)
                {
                    int16_t platoon_index = squad_def->platoon_index;
                    int position_mask = 0;
                    uint8_t defending = 0;
                    BIT_VECTOR_SET_FLAG(squads_migrating, s);
                    if (platoon_index >= 0 && platoon_index < encounter_def->platoons.count)
                        defending = platoons[(int16_t)((uint16_t)encounter->platoon_base + platoon_index)].defending;
                    if (defending)
                    {
                        for (int16_t g = _firing_position_group_defending; g <= _firing_position_group_defending_guard; ++g)
                            position_mask |= squad_def->firing_position_groups[g];
                    }
                    else
                    {
                        for (int16_t g = _firing_position_group_attacking; g <= _firing_position_group_attacking_guard; ++g)
                            position_mask |= squad_def->firing_position_groups[g];
                    }
                    reachable_group_mask |= position_mask;
                    squad_position_masks[s] = position_mask;
                    if (current_count > 0)
                        BIT_VECTOR_SET_FLAG(squads_with_living, s);
                }
            }
        }
    }

    if (!(total_living_count > 0 && reachable_group_mask))
        return;

    int16_t nearest_squad;
    int16_t farthest_squad;

    if (target_count == 1)
    {
        best_target_unit = target_units[0];
        object_get_origin(target_units[0], &follow_origin);
        goto resolve_migration;
    }

    /* multi-target: seed per-target min-distance and origins, then accumulate per-actor squared distance */
    for (int16_t m = 0; m < target_count; ++m)
    {
        target_min_d2[m] = 3.4028235e38f; /* FLT_MAX */
        object_get_origin(target_units[m], &target_origins[m]);
    }

    {
        int ai_initialized = ai_globals->ai_initialized_for_map;
        int actor_index;
        if (ai_initialized)
        {
            if (encounter_index == -1)
                actor_index = ai_globals->first_encounterless_actor_index;
            else
                actor_index = encounter->first_actor_index;
        }
        else
        {
            /* DEVIATION: decompiler read uninitialized stack here; value is never used because
             * the loop below guards on ai_initialized. Modelled as the -1 terminator. */
            actor_index = -1;
        }

        while (ai_initialized && actor_index != -1)
        {
            actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
            actor_index = actor->meta.next_actor_index;
            int16_t squad_index = actor->meta.squad_index;
            if (BIT_VECTOR_TEST_FLAG(squads_present, squad_index) && target_count > 0)
            {
                float ax = actor->input.position.body_position.x;
                float ay = actor->input.position.body_position.y;
                float az = actor->input.position.body_position.z;
                for (int16_t m = 0; m < target_count; ++m)
                {
                    float dx = target_origins[m].n[0] - ax;
                    float dy = target_origins[m].n[1] - ay;
                    float dz = target_origins[m].n[2] - az;
                    float d2 = dx * dx + (dz * dz + dy * dy);
                    if (target_min_d2[m] > d2)
                        target_min_d2[m] = d2;
                }
            }
        }
    }

    /* collapse to a single origin: the LAST target with any finite actor distance wins
     * (no distance comparison in the binary — preserved) */
    for (int16_t n = 0; n < target_count; ++n)
    {
        if (target_min_d2[n] < 3.4028235e38f) /* FLT_MAX */
        {
            best_target_unit = target_units[n];
            follow_origin.n[0] = target_origins[n].n[0];
            follow_origin.n[1] = target_origins[n].n[1];
            follow_origin.n[2] = target_origins[n].n[2];
        }
    }

resolve_migration:
    {
        ai_globals_t *globals = ai_globals;
        if (best_target_unit == -1)
            return;

        /* per firing-position letter group: nearest squared distance from the chosen target origin */
        nearest_squad = -1;
        for (int16_t g = 0; g < NUMBER_OF_FIRING_POSITION_GROUP_INDICES; ++g)
            group_min_d2[g] = 3.4028235e38f; /* FLT_MAX */

        int position_count = encounter_def->firing_positions.count;
        if (position_count > 0)
        {
            firing_position_definition *positions = (firing_position_definition *)encounter_def->firing_positions.address;
            float ox = follow_origin.n[0];
            float oy = follow_origin.n[1];
            float oz = follow_origin.n[2];
            for (int16_t p = 0; p < position_count; ++p)
            {
                firing_position_definition *pos = &positions[p];
                int group_index = pos->group_index;
                if (((1 << group_index) & reachable_group_mask) != 0)
                {
                    float dx = ox - pos->position.x;
                    float dy = oy - pos->position.y;
                    float dz = oz - pos->position.z;
                    float d2 = dx * dx + (dz * dz + dy * dy);
                    if (group_min_d2[group_index] > d2)
                        group_min_d2[group_index] = d2;
                }
            }
        }

        /* choose the migrating squad whose nearest allowed group is closest (nearest_squad)
         * vs. the living squad whose nearest allowed group is farthest (farthest_squad) */
        int16_t squads_n = encounter->squad_count;
        float closest_d2 = 3.4028235e38f;  /* FLT_MAX */
        float farthest_d2 = -3.4028235e38f;
        farthest_squad = -1;
        for (int16_t s = 0; s < squads_n; ++s)
        {
            if (BIT_VECTOR_TEST_FLAG(squads_migrating, s))
            {
                float squad_min_d2 = 3.4028235e38f; /* FLT_MAX */
                for (int16_t k = 0; k < NUMBER_OF_FIRING_POSITION_GROUP_INDICES; ++k)
                {
                    if (((1 << k) & squad_position_masks[s]) != 0 && squad_min_d2 > group_min_d2[k])
                        squad_min_d2 = group_min_d2[k];
                }
                if (squad_min_d2 < closest_d2)
                {
                    nearest_squad = s;
                    closest_d2 = squad_min_d2;
                }
                if (BIT_VECTOR_TEST_FLAG(squads_with_living, s) && squad_min_d2 > farthest_d2)
                {
                    farthest_squad = s;
                    farthest_d2 = squad_min_d2;
                }
            }
        }

        if (nearest_squad != -1 && farthest_squad != -1)
        {
            float threshold = encounter->follow_target_distance;
            float near_d = sqrtf(closest_d2);   /* __fsqrts */
            float far_d = sqrtf(farthest_d2);   /* __fsqrts */
            if (threshold <= 0.0f)
                threshold = 2.0f;
            if (near_d >= (far_d - threshold))
                nearest_squad = -1;
        }

        if (nearest_squad != -1)
        {
            int actor_index;
            if (globals->ai_initialized_for_map)
            {
                if (encounter_index == -1)
                    actor_index = globals->first_encounterless_actor_index;
                else
                    actor_index = encounter->first_actor_index;
            }
            else
            {
                /* DEVIATION: decompiler read uninitialized stack here; value is never used because
                 * the loop below guards on ai_initialized_for_map. Modelled as the -1 terminator. */
                actor_index = -1;
            }

            while (globals->ai_initialized_for_map)
            {
                int this_actor = actor_index;
                if (this_actor == -1)
                    break;
                actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, this_actor);
                actor_index = actor->meta.next_actor_index;
                int16_t squad_index = actor->meta.squad_index;
                if (BIT_VECTOR_TEST_FLAG(squads_present, squad_index) && squad_index != nearest_squad)
                {
                    actor_change_encounter(this_actor, encounter_index, nearest_squad);
                    globals = ai_globals;
                }
            }
        }
    }
}
