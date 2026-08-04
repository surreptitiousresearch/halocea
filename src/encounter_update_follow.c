/* encounter_update_follow @0x8370C418 — migrate an encounter's squads between firing positions to
 * "follow" a set of target units (the player, a specific unit, or the actors of a referenced AI index).
 *
 * Phase 1: build the follow-target list (up to 8) from the encounter's follow mode (word49):
 *   _follow_target_players = follow players, _follow_target_unit = follow a specific unit,
 *   _follow_target_ai = follow the units of a referenced AI index.
 * Phase 2: for each squad flagged "migrating" in its current firing-position set, accumulate the squad's
 *   present/migration-target bitmasks and total living count.
 * Phase 3: find, per target, the nearest reachable firing position to each squad, then pick the squad
 *   whose nearest target position is closest (v54) vs. farthest (v71); if they differ by more than the
 *   encounter's migration threshold, migrate every actor of the chosen squad into firing-position v54.
 *
 * Reconstructed from the decompiler. The decompiler overlays one stack int array (scratch[110]) for the
 * target indices, per-target min-distances (as floats), origins, and per-squad bitmasks; that overlay is
 * preserved verbatim. PPC `__fsqrts` is written as sqrtf (marked). The actor-list walk passes the actor
 * index through a float register in the decompiler; modelled here as a plain int (equivalent).
 */

#include <stdint.h>
#include "headers/encounter_datum.h"
#include "headers/ai_follow_target_type.h"
#include "headers/squad_flags.h"
#include "headers/encounter_definition.h"
#include "headers/squad_definition.h"
#include "headers/firing_position_definition.h"
#include <math.h>
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
#include "headers/blam_data_globals.h"

/* encounter_data / actor_data come from blam_data_globals.h (data_array *) */
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern void actor_change_encounter(int actor_index, int encounter_index, int16_t squad_index);
extern void ai_index_actor_iterator_new(unsigned int ai_index, ai_index_actor_iterator *iterator);
extern actor_datum *ai_index_actor_iterator_next(ai_index_actor_iterator *iterator);

void encounter_update_follow(int encounter_index)
{
    encounter_datum *encounter;      /* v2 */
    encounter_definition *scenario_def; /* v3 */
    int follow_mode;                 /* v4 */
    int best_position;               /* v6 */
    __int16 target_count;            /* v7 / v14 */
    void *iter_actor;
    int *iter_player;
    real_point3d origin;             /* v86 */
    __int64 migration_target_mask;   /* v87 — squads whose firing set wants migration */
    __int64 present_mask;            /* v88 — squads present this update */
    ai_index_actor_iterator follow_iter; /* v89 — also reused as a third (has-count) bitmask base */
    int scratch[110];                /* v90 — overlay: [0..7] target idx, [8..15]/[16..] dist & origins, [44..] squad masks */

    encounter = DATUM_GET(encounter_data, encounter_datum, encounter_index);
    scenario_def = (encounter_definition *)global_scenario->ai_encounters.address + (unsigned __int16)encounter_index;
    follow_mode = (unsigned __int16)encounter->follow_target_type;
    best_position = -1;
    target_count = 0;

    switch (follow_mode)
    {
        case _follow_target_players: /* follow players */
            /* follow_iter is a single stack slot punned three ways (data_iterator here,
             * ai_index_actor_iterator in the AI case, mask base below); faithful to the
             * original stack overlay, so the data_iterator cast is load-bearing. */
            data_iterator_new((data_iterator *)&follow_iter, player_data);
            for (iter_player = data_iterator_next((data_iterator *)&follow_iter); iter_player;
                 iter_player = data_iterator_next((data_iterator *)&follow_iter))
            {
                int unit_index = iter_player[13];
                if (unit_index != -1)
                {
                    int slot = target_count;
                    if (target_count < 8)
                    {
                        ++target_count;
                        scratch[slot] = unit_index;
                    }
                }
            }
            break;
        case _follow_target_unit: /* follow a specific unit */
            if (object_try_and_get_and_verify_type(encounter->___u43.follow_target_unit_index, object_mask_unit))
            {
                target_count = 1;
                scratch[0] = encounter->___u43.follow_target_unit_index;
            }
            else
            {
                encounter->___u43.follow_target_unit_index = -1;
            }
            break;
        case _follow_target_ai: /* follow the units of a referenced AI index */
        {
            int ai_index = encounter->___u43.follow_target_ai_index;
            if (ai_index != -1)
            {
                ai_index_actor_iterator_new(ai_index, &follow_iter);
                for (iter_actor = ai_index_actor_iterator_next(&follow_iter); iter_actor;
                     iter_actor = ai_index_actor_iterator_next(&follow_iter))
                {
                    if (target_count >= 8)
                        break;
                    int slot = target_count++;
                    scratch[slot] = ((actor_datum *)iter_actor)->meta.unit_index;
                }
            }
            break;
        }
    }

    target_count = target_count; /* v14 = v7 */
    if (target_count <= 0)
        return;

    __int16 squad_count = encounter->squad_count;
    int total_count = 0;             /* v17 */
    int reachable_position_mask = 0; /* v16 */
    present_mask = 0;                /* v88 — decompiler reuses register, semantically a zeroed mask */
    migration_target_mask = 0;       /* v87 */
    *(__int64 *)&follow_iter.encounter_index = 0; /* third mask: squads with living count */
    int local_squad = 0;             /* v18 */

    if (squad_count > 0)
    {
        squad_definition *squad_defs = (squad_definition *)scenario_def->squads.address;
        int squad_limit = squad_count;
        int s = 0;
        squad_datum *squads = squad_array;
        platoon_datum *platoons = platoon_array;
        do
        {
            squad_definition *rule = &squad_defs[s];
            if ((rule->flags & (1u << _squad_automatic_migration_bit)) != 0)       /* squad is migrating */
            {
                int dword_index = 4 * (s >> 5);
                int bit = 1 << (s & 0x1F);
                squad_datum *squad = &squads[(__int16)((unsigned __int16)encounter->squad_base + local_squad)];
                *(int *)((char *)&present_mask + dword_index) |= bit;
                int current_count = squad->current_count;
                total_count += current_count;
                if (squad->automatic_migration_target)
                {
                    __int16 platoon_local = rule->platoon_index;
                    int position_mask = 0;
                    unsigned __int8 defending = 0;
                    *(int *)((char *)&migration_target_mask + dword_index) |= bit;
                    if (platoon_local >= 0 && platoon_local < scenario_def->platoons.count)
                        defending = platoons[(__int16)((unsigned __int16)encounter->platoon_base + platoon_local)].defending;
                    if (defending)
                    {
                        int p = 3;
                        do
                        {
                            position_mask |= rule->firing_position_groups[p];
                            p = (__int16)(p + 1);
                        } while (p <= 5);
                    }
                    else
                    {
                        int p = 0;
                        do
                        {
                            position_mask |= rule->firing_position_groups[p];
                            p = (__int16)(p + 1);
                        } while (p <= 2);
                    }
                    reachable_position_mask |= position_mask;
                    scratch[s + 44] = position_mask;
                    if (current_count > 0)
                        *(int *)((char *)&follow_iter.encounter_index + dword_index) |= bit;
                }
            }
            local_squad = (__int16)(s + 1);
            s = local_squad;
        } while (local_squad < squad_limit);
    }

    if (!(total_count > 0 && reachable_position_mask))
        return;

    __int16 nearest_squad;   /* v54 */
    __int16 farthest_squad;  /* v71 */

    if (target_count == 1)
    {
        best_position = scratch[0];
        object_get_origin(scratch[0], &origin);
        goto resolve_migration;
    }

    /* multi-target: seed per-target min-distance and origins, then accumulate squared distance */
    if (target_count > 0)
    {
        for (int m = 0; m < target_count; m = (__int16)(m + 1))
        {
            *(float *)&scratch[m + 8] = 3.4028235e38f;
            object_get_origin(scratch[m], (real_point3d *)&scratch[3 * m + 16]);
        }
    }

    {
        ai_globals_t *globals = ai_globals;
        int ai_initialized = ai_globals->ai_initialized_for_map;
        int actor_iter;
        if (ai_globals->ai_initialized_for_map)
        {
            if (encounter_index == -1)
                actor_iter = ai_globals->first_encounterless_actor_index;
            else
                actor_iter = encounter->first_actor_index;
        }
        else
        {
            actor_iter = *(int *)&origin.n[2]; /* uninitialized scratch — preserved decompiler artifact (float bits as int) */
        }

        while (ai_initialized && actor_iter != -1)
        {
            actor_datum *actor_entry = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_iter);
            actor_iter = actor_entry->meta.next_actor_index;      /* recovered: *((int *)actor_entry + 11) */
            __int16 squad_local = actor_entry->meta.squad_index;  /* recovered: *((__int16 *)actor_entry + 29) */
            if (((1 << (squad_local & 0x1F)) & *((int *)&present_mask + (squad_local >> 5))) != 0 && target_count > 0)
            {
                float ax = actor_entry->input.position.body_position.x;
                float ay = actor_entry->input.position.body_position.y;
                float az = actor_entry->input.position.body_position.z;
                int m = 0;
                do
                {
                    float *target_origin = (float *)&scratch[3 * m + 16];
                    float dist = *(float *)&scratch[m + 8];
                    float dy = target_origin[1] - ay;
                    float dz = target_origin[2] - az;
                    float dx = target_origin[0] - ax;
                    float d2 = dx * dx + (dz * dz + dy * dy);
                    if (dist > d2)
                        dist = d2;
                    *(float *)&scratch[m + 8] = dist;
                    m = (__int16)(m + 1);
                } while (m < target_count);
            }
        }
    }

    /* collapse the per-target distances to a single best origin (closest target unit) */
    if (target_count > 0)
    {
        for (int n = 0; n < target_count; n = (__int16)(n + 1))
        {
            if (*(float *)&scratch[n + 8] < 3.4028235e38f)
            {
                float *target_origin = (float *)&scratch[3 * n + 16];
                best_position = scratch[n];
                float oy = target_origin[1];
                float oz = target_origin[2];
                origin.n[0] = target_origin[0]; /* was LODWORD(origin.n[0])=*(int*)target_origin — same 32-bit float copy */
                origin.n[1] = oy;
                origin.n[2] = oz;
            }
        }
        goto resolve_migration;
    }
    return;

resolve_migration:
    {
        ai_globals_t *globals = ai_globals;
        if (best_position == -1)
            return;

        /* per-firing-position nearest squared distance from the chosen target origin */
        __int16 idx16 = 0;
        nearest_squad = -1;
        for (int i2 = 0; i2 < 26; i2 = (__int16)(i2 + 1))
            *(float *)&scratch[i2 + 16] = 3.4028235e38f;

        int position_count = scenario_def->firing_positions.count;
        if (position_count > 0)
        {
            firing_position_definition *positions = (firing_position_definition *)scenario_def->firing_positions.address;
            float oz = origin.n[2];
            float oy = origin.n[1];
            float ox = origin.n[0];
            int p = 0;
            do
            {
                firing_position_definition *pos = &positions[p];
                int squad_bit_index = pos->group_index;
                if (((1 << squad_bit_index) & reachable_position_mask) != 0)
                {
                    float cur = *(float *)&scratch[squad_bit_index + 16];
                    float dy = oy - pos->position.y;
                    float dz = oz - pos->position.z;
                    float dx = ox - pos->position.x;
                    float d2 = dx * dx + (dz * dz + dy * dy);
                    if (cur > d2)
                        cur = d2;
                    *(float *)&scratch[squad_bit_index + 16] = cur;
                }
                p = (__int16)(p + 1);
            } while (p < position_count);
        }

        /* choose the migrating squad whose nearest position is closest (nearest_squad)
         * vs. the present squad whose nearest position is farthest (farthest_squad) */
        __int16 squads_n = encounter->squad_count;
        float closest = 3.4028235e38f;
        farthest_squad = -1;
        int sq16 = 0;
        float farthest = -3.4028235e38f;
        if (squads_n > 0)
        {
            int s = 0;
            int limit = squads_n;
            do
            {
                int dword_index = 4 * (s >> 5);
                int bit = 1 << (s & 0x1F);
                if ((*(int *)((char *)&migration_target_mask + dword_index) & bit) != 0)
                {
                    float squad_min = 3.4028235e38f;
                    for (int k = 0; k < 26; k = (__int16)(k + 1))
                    {
                        if (((1 << k) & scratch[s + 44]) != 0 && squad_min > *(float *)&scratch[k + 16])
                            squad_min = *(float *)&scratch[k + 16];
                    }
                    if (squad_min < closest)
                    {
                        nearest_squad = sq16;
                        closest = squad_min;
                    }
                    if ((*(int *)((char *)&follow_iter.encounter_index + dword_index) & bit) != 0 && squad_min > farthest)
                    {
                        farthest_squad = sq16;
                        farthest = squad_min;
                    }
                }
                sq16 = (__int16)(s + 1);
                s = sq16;
            } while (sq16 < limit);
        }

        if (nearest_squad != -1 && farthest_squad != -1)
        {
            float threshold = encounter->follow_target_distance;
            float near_d = sqrtf(closest);   /* __fsqrts */
            float far_d = sqrtf(farthest);   /* __fsqrts */
            if (threshold <= 0.0f)
                threshold = 2.0f;
            if (near_d >= (far_d - threshold))
                nearest_squad = -1;
        }

        if (nearest_squad != -1)
        {
            int actor_iter;
            if (globals->ai_initialized_for_map)
            {
                if (encounter_index == -1)
                    actor_iter = globals->first_encounterless_actor_index;
                else
                    actor_iter = encounter->first_actor_index;
            }
            else
            {
                actor_iter = *(int *)&origin.n[2]; /* uninitialized scratch — preserved decompiler artifact (float bits as int) */
            }

            while (globals->ai_initialized_for_map)
            {
                int this_actor = actor_iter;
                if (this_actor == -1)
                    break;
                actor_datum *actor_entry = DATA_ARRAY_ELEMENT(actor_data, actor_datum, this_actor);
                actor_iter = actor_entry->meta.next_actor_index;      /* recovered: actor_entry[11] */
                __int16 squad_local = actor_entry->meta.squad_index;  /* recovered: *((__int16 *)actor_entry + 29) */
                if (((1 << (squad_local & 0x1F)) & *((int *)&present_mask + (squad_local >> 5))) != 0
                    && squad_local != nearest_squad)
                {
                    actor_change_encounter(this_actor, encounter_index, nearest_squad);
                    globals = ai_globals;
                }
            }
        }
    }
}
