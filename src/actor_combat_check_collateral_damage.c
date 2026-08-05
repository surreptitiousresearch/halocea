/* actor_combat_check_collateral_damage @0x837B83E0 — decide whether a grenade thrown to land at test_point is
 * worth throwing: it tallies a "threat score" of enemies caught inside enemy_radius and rejects the throw if a
 * friendly (the actor itself or an ally) would fall inside collateral_damage_radius.
 *
 * Enemy tally (written through threat_count_out): players count 10, vehicle-mounted enemies count 5, swarms
 * count their member count, other actors count 1. Enemies found via the actor's prop list are remembered (up
 * to 32 actor indices) so the subsequent encounter sweep doesn't double-count them.
 *
 * Returns 1 if the throw is acceptable, 0 if a friendly is inside the collateral radius.
 *
 * Deviation: the decompiler shows 7 parameters; the prologue shows enemy_radius/collateral_damage_radius are
 * floats (f1/f2) whose GPR-skip reserves r4/r5, so the decompiler's 4th/5th params (target_point,
 * enemy_count_reference) are phantoms. The real arguments are (actor_index, enemy_radius, collateral_radius,
 * test_point (r6), threat_count_out (r7)). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/prop_datum.h"
#include "headers/prop_iterator.h"
#include "headers/actor_datum.h"
#include "headers/encounter_actor_iterator.h"
#include "headers/real_point3d.h"
#include "headers/prop_perception_state.h"
#include "headers/blam_data_globals.h"
extern void prop_iterator_new(prop_iterator *iterator, uint16_t actor_index);

extern prop_datum *prop_iterator_next(prop_iterator *iterator);


uint8_t actor_combat_check_collateral_damage(int actor_index, float enemy_radius, float collateral_damage_radius,
                                             const float *test_point, int16_t *threat_count_out)
{
    int acceptable = 1;
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int16_t threat_count = 0;
    uint16_t enemy_actor_count = 0;
    int enemy_actor_indices[32];

    /* --- pass 1: the actor's own props (units it is aware of) --- */
    prop_iterator prop_cursor;
    prop_iterator_new(&prop_cursor, actor_index);
    for ( prop_datum *prop = prop_iterator_next(&prop_cursor); prop; prop = prop_iterator_next(&prop_cursor) )
    {
        if ( prop->state < _prop_state_becoming_unacknowledged || prop->state > _prop_state_acknowledged || prop->dead )
            continue;

        if ( prop->enemy )
        {
            float dx = test_point[0] - prop->body_position.n[0];
            float dz = test_point[2] - prop->body_position.n[2];
            float dy = test_point[1] - prop->body_position.n[1];
            if ( (dy * dy) + ((dz * dz) + (dx * dx)) < enemy_radius * enemy_radius )
            {
                if ( prop->player )
                {
                    threat_count += 10;
                }
                else if ( prop->vehicle_index != -1 )
                {
                    threat_count += 5;
                }
                else
                {
                    int enemy_actor_index = prop->actor_index;
                    if ( enemy_actor_index != -1 )
                    {
                        if ( enemy_actor_count < 0x20u )
                            enemy_actor_indices[enemy_actor_count++] = enemy_actor_index;
                        if ( prop->swarm )
                            threat_count += DATA_ARRAY_ELEMENT(actor_data, actor_datum, enemy_actor_index)->meta.swarm_unit_count;
                        else
                            ++threat_count;
                    }
                }
            }
        }
        else if ( collateral_damage_radius > 0.0f )
        {
            float dy = test_point[1] - prop->body_position.n[1];
            float dz = test_point[2] - prop->body_position.n[2];
            float dx = test_point[0] - prop->body_position.n[0];
            if ( (dx * dx) + ((dz * dz) + (dy * dy)) < collateral_damage_radius * collateral_damage_radius )
            {
                acceptable = 0;
                break;
            }
        }
    }

    /* --- pass 2: enemies in the encounter of the actor's "interesting" prop (actor+624), excluding any
     *     already counted from the prop list --- */
    int interesting_prop_index = actor->target.target_prop_index;
    if ( enemy_radius > 0.0f && interesting_prop_index != -1 )
    {
        int prop_actor_index = (DATA_ARRAY_ELEMENT(prop_data, prop_datum, interesting_prop_index))->actor_index;
        if ( prop_actor_index != -1 )
        {
            int encounter_index = DATA_ARRAY_ELEMENT(actor_data, actor_datum, prop_actor_index)->meta.encounter_index;
            if ( encounter_index != -1 )
            {
                encounter_actor_iterator iterator;
                encounter_actor_iterator_new(&iterator, encounter_index);
                for ( actor_datum *encounter_actor = encounter_actor_iterator_next(&iterator);
                      encounter_actor;
                      encounter_actor = encounter_actor_iterator_next(&iterator) )
                {
                    char already_counted = 0;
                    if ( (int16_t)enemy_actor_count > 0 )
                    {
                        int16_t i = 0;
                        while ( enemy_actor_indices[i] != iterator.index )
                        {
                            ++i;
                            if ( i >= (int16_t)enemy_actor_count )
                                goto checked;
                        }
                        already_counted = 1;
                    }
                checked:
                    if ( !already_counted )
                    {
                        float dy = test_point[1] - encounter_actor->input.position.body_position.n[1];
                        float dz = test_point[2] - encounter_actor->input.position.body_position.n[2];
                        float dx = test_point[0] - encounter_actor->input.position.body_position.n[0];
                        if ( (dx * dx) + ((dz * dz) + (dy * dy)) < enemy_radius * enemy_radius )
                        {
                            if ( encounter_actor->meta.swarm )
                                threat_count += encounter_actor->meta.swarm_unit_count;
                            else
                                ++threat_count;
                        }
                    }
                }
            }
        }
    }

    /* --- pass 3: friendlies in the actor's own encounter inside the collateral radius abort the throw --- */
    if ( acceptable )
    {
        int own_encounter_index = actor->meta.encounter_index;
        if ( own_encounter_index != -1 && collateral_damage_radius > 0.0f )
        {
            encounter_actor_iterator iterator;
            encounter_actor_iterator_new(&iterator, own_encounter_index);
            float collateral_radius_sq = collateral_damage_radius * collateral_damage_radius;
            for ( actor_datum *encounter_actor = encounter_actor_iterator_next(&iterator);
                  encounter_actor;
                  encounter_actor = encounter_actor_iterator_next(&iterator) )
            {
                float dy = test_point[1] - encounter_actor->input.position.body_position.n[1];
                float dz = test_point[2] - encounter_actor->input.position.body_position.n[2];
                float dx = test_point[0] - encounter_actor->input.position.body_position.n[0];
                if ( (dx * dx) + ((dz * dz) + (dy * dy)) < collateral_radius_sq )
                {
                    acceptable = 0;
                    break;
                }
            }
        }
    }

    if ( threat_count_out )
        *threat_count_out = threat_count;
    return acceptable;
}
